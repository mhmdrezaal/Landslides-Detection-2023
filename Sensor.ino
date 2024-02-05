void setup_gyro() {

  // Coba untuk menginisialisasi sensor MPU6050
  if (!mpu.begin()) {
    Serial.println("Gagal menemukan chip MPU6050");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 ditemukan!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);       // Set pengaturan rentang akselerometer
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);            // Set pengaturan rentang giroskop
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);          // Set bandwidth filter

  // Inisialisasi array pembacaan
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

float calculateWeightedAverage(int values[], int size) {
  int sum = 0;
  int sumWeights = 0;

  for (int i = 0; i < size; i++) {
    sum += values[i];
    sumWeights += i * values[i];  // Weighted sum
  }

  if (sum == 0) return 0;

  return float(sumWeights) / float(sum);
}

void read_sensor() {
  // Dapatkan data sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

   // Kalibrasi sumbu X dan Y
  a.acceleration.x = abs(a.acceleration.x) + calibrateX;
  a.acceleration.y = abs(a.acceleration.y) + calibrateY;

  // Hitung sudut kemiringan dalam derajat
  float xAngle = atan2(abs(a.acceleration.x), a.acceleration.z) * 180.0 / PI;
  float yAngle = atan2(abs(a.acceleration.y), a.acceleration.z) * 180.0 / PI;
  mpux = xAngle;
  mpuy = yAngle;
}

void measure_rh(){
  int sensorValue = analogRead(soilPin);
  total -= readings[currentIndex];
  readings[currentIndex] = sensorValue;
  total += sensorValue;
  currentIndex = (currentIndex + 1) % numReadings;
  int average = total / numReadings;
  int invertedPercentage = 100 - map(average, 0, 4095, 0, 100);
  rh = invertedPercentage;
}

void measure_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  pinMode(echoPin, INPUT); 
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  distance = distanceCm;
  
  //convert to inches
  //distanceInch = distanceCm * CM_TO_INCH;
}
