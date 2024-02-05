#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;

// LED pins
const int blueLED = 13;
const int greenLED = 12;
const int yellowLED = 14;
const int redLED = 0;

// Kalibrasi sumbu X dan Y
const float calibrateX = 0;  // Kalibrasi sumbu X
const float calibrateY = 0;  // Kalibrasi sumbu Y

const int soilPin = 32;
const int numReadings = 10;  // Jumlah pembacaan yang akan diambil
int readings[numReadings];   // Simpan pembacaan sensor dalam array
int currentIndex = 0;        // Indeks pembacaan saat ini
int total = 0;               // Total pembacaan

// Ultrasonic Distance HC-SR04
const int trigPin           = 26; 
const int echoPin           = 25;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;


// Variable Membership PID

#define mem_mpux 4        
#define mem_mpuy 4
#define mem_distance 4
#define mem_rh 4

// Nilai min dan max variabel
#define min_mpux 0
#define max_mpux 90

#define min_mpuy 0
#define max_mpuy 90

#define min_distance 0
#define max_distance 200

#define min_rh 0
#define max_rh 100

// Index posisi dari masing-masing mF Variabel
#define hold_mpu 0
#define low_mpu 1
#define med_mpu 2
#define high_mpu 3

#define lowdn_distance 0
#define low_distance 1
#define med_distance 2
#define highdn_distance 3

#define dry_rh 0
#define normal_rh 1
#define wet_rh 2
#define vwet_rh 3

#define normal 0
#define siaga 1
#define waspada 2
#define awas 3


//deklarasi membership fuzzy
#define jumlah_rule 28
double rule[jumlah_rule + 1];
double bobot_mpux[mem_mpux];
double bobot_mpuy[mem_mpuy];
double bobot_distance[mem_distance];
double bobot_rh[mem_rh];

//+++++membership function LDR IN
double mf_sen_mpux[mem_mpux][3] = {
  { 0, 0, 1 },      //diam
  { 0, 9, 18 },    //ringan
  { 13, 20, 27 },   //sedang
  { 22, 29, 90 },  //tinggi
};

//+++++membership function LDR OUT

double mf_sen_mpuy[mem_mpuy][3] = {
  { 0, 0, 1 },      //diam
  { 0, 9, 18 },    //ringan
  { 13, 20, 27 },   //sedang
  { 22, 29, 90 },  //tinggi
};


double mf_sen_distance[mem_distance][3] = {
  { 0, 70, 90 },    //dekat
  { 70, 90, 115 },   //sedang
  { 90, 110, 130 },  //jauh
  { 115, 130, 200 },  //diam
};

double mf_sen_rh[mem_rh][3] = {
  { 0, 25, 35 },    //kering
  { 15, 45, 75 },   //normal
  { 45, 75, 95 },  //basah
  { 85, 95, 100 },  //sangat_basah
};

//+++++membership function Duty Cycle
double mf_status[] = { 0, 50, 75, 100 };

long old_millis;
int total_rule;
bool mode;
int jml_loop_mpux, jml_loop_mpuy, jml_loop_tegangan;
float mpux = 0;
float mpuy = 0;
float distance = 0;
float rh = 0;
long last_millis_wifi = 0;
int ulang;
String status = "AMAN";

void setup() {
  Serial.begin(115200);
  setup_gyro();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(soilPin, INPUT);

  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

long last_on;
int minute, second;


void loop() {

  read_sensor();
  measure_rh();
  measure_distance();

  // data dummy
  //mpux = 0.52;      // data dummy
  //mpuy = 1.07;      // data dummy
  //distance = 100;  // data dummy
  //rh = 40;        // data dummy


  if (mpux < min_mpux) {
    mpux = min_mpux;
  } else if (mpux > max_mpux) {
    mpux = max_mpux;
  }

  if (mpuy < min_mpuy) {
    mpuy = min_mpuy;
  } else if (mpuy > max_mpuy) {
    mpuy = max_mpuy;
  }

  if (distance < min_distance) {
    distance = min_distance;
  } else if (distance > max_distance) {
    distance = max_distance;
  }

  if (rh < min_rh) {
    rh = min_rh;
  } else if (rh > max_rh) {
    rh = max_rh;
  }

  //FuzziFikasi
  
  get_bobot_mpux(mpux, mem_mpuy);
  get_bobot_mpuy(mpuy, mem_mpuy);
  get_bobot_distance(distance, mem_distance);
  get_bobot_rh(rh, mem_rh);

  //Rulebase
  rule[1] = min_val4(bobot_mpux[hold_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance], bobot_rh[dry_rh]);
  rule[2] = min_val4(bobot_mpux[hold_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance], bobot_rh[normal_rh]);
  rule[3] = min_val4(bobot_mpux[hold_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance], bobot_rh[wet_rh]);
  rule[4] = min_val4(bobot_mpux[hold_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance], bobot_rh[vwet_rh]);
  rule[5] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance]);
  rule[6] = min_val3(bobot_mpux[med_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance]);
  rule[7] = min_val3(bobot_mpux[high_mpu], bobot_mpuy[hold_mpu], bobot_distance[low_distance]);
  
  rule[8] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[low_mpu], bobot_distance[low_distance]);
  rule[9] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[med_mpu], bobot_distance[low_distance]);
  rule[10] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[high_mpu], bobot_distance[low_distance]);
  rule[11] = min_val1(bobot_distance[highdn_distance]);
  rule[12] = min_val1(bobot_distance[lowdn_distance]);
  rule[13] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[hold_mpu], bobot_distance[med_distance]);
  rule[14] = min_val3(bobot_mpux[med_mpu], bobot_mpuy[hold_mpu], bobot_distance[med_distance]);
  
  rule[15] = min_val3(bobot_mpux[high_mpu], bobot_mpuy[hold_mpu], bobot_distance[med_distance]);
  rule[16] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[low_mpu], bobot_distance[med_distance]);
  rule[17] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[med_mpu], bobot_distance[med_distance]);
  rule[18] = min_val3(bobot_mpux[hold_mpu], bobot_mpuy[high_mpu], bobot_distance[med_distance]);
  rule[19] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[high_mpu], bobot_distance[highdn_distance]);
  rule[20] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[high_mpu], bobot_distance[low_distance]);
  rule[21] = min_val3(bobot_mpux[high_mpu], bobot_mpuy[low_mpu], bobot_distance[low_distance]);
  
  rule[22] = min_val3(bobot_mpux[med_mpu], bobot_mpuy[med_mpu], bobot_distance[low_distance]);
  rule[23] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[low_mpu], bobot_distance[low_distance]);
  rule[24] = min_val3(bobot_mpux[high_mpu], bobot_mpuy[high_mpu], bobot_distance[low_distance]);
  rule[25] = min_val3(bobot_mpux[low_mpu], bobot_mpuy[med_mpu], bobot_distance[low_distance]);
  rule[26] = min_val3(bobot_mpux[med_mpu], bobot_mpuy[low_mpu], bobot_distance[low_distance]);
  rule[27] = min_val3(bobot_mpux[med_mpu], bobot_mpuy[high_mpu], bobot_distance[low_distance]);
  rule[28] = min_val3(bobot_mpux[high_mpu], bobot_mpuy[med_mpu], bobot_distance[low_distance]);

  //defuzzi
  double total_rule = 0;
  for (int xx = 1; xx <= jumlah_rule; xx++) {
    total_rule = rule[xx] + total_rule;
  }

  double defuzifikasi = 0;
   defuzifikasi = rule[1] * mf_status[normal] + rule[2] * mf_status[normal] + rule[3] * mf_status[normal] + rule[4] * mf_status[siaga] + rule[5] * mf_status[siaga] + rule[6] * mf_status[waspada] + rule[7] * mf_status[awas] + rule[8] * mf_status[siaga] + rule[9] * mf_status[waspada] + rule[10] * mf_status[awas] + rule[11] * mf_status[awas] + rule[12] * mf_status[awas] + rule[13] * mf_status[waspada] + rule[14] * mf_status[waspada] + rule[15] * mf_status[awas] +  rule[16] * mf_status[waspada] + 
   rule[17] * mf_status[waspada] + rule[18] * mf_status[awas] + rule[19] * mf_status[awas] + rule[20] * mf_status[awas] +  rule[21] * mf_status[awas] + rule[22] * mf_status[waspada] + rule[23] * mf_status[siaga] + rule[24] * mf_status[awas] + rule[25] * mf_status[waspada] + rule[26] * mf_status[waspada] + rule[27] * mf_status[awas] + rule[28] * mf_status[awas];

 
  defuzifikasi = defuzifikasi / total_rule;
  if (isnan(defuzifikasi)) {
    defuzifikasi = 0;
  }

  if (defuzifikasi < 49) {
    status = "1";
  } else if (defuzifikasi < 74) {
    status = "2";
  } else if (defuzifikasi < 100) {
    status = "3";
  } else  {
    status = "4";
  }

  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  if (status == "1") {
    digitalWrite(blueLED, HIGH);  // LED1 nyala
  } else if (status == "2") {
    digitalWrite(greenLED, HIGH);  // LED2 nyala
  } else if (status == "3") {
    digitalWrite(yellowLED, HIGH);  // LED3 nyala
  } else if (status == "4") {
    digitalWrite(redLED, HIGH);  // LED4 nyala
  }

  Serial.print("MPUX:");
  Serial.println(mpux);
  Serial.print("MPUY:");
  Serial.println(mpuy);
  Serial.print("Distance:");
  Serial.println(distance);
  Serial.print("Hummidity:");
  Serial.println(rh);

  Serial.print("defuzifikasi:");
  Serial.println(defuzifikasi);
  Serial.print("Status:");
  Serial.println(status);
  Serial.println("==========================================");
  delay(1000);
}
