double min_val1(double sen1){
  return sen1;
}

double min_val2(double sen1, double sen2) {
  double x = minn(sen1, sen2);
  return x;
}

double min_val3(double sen1, double sen2, double sen3) {
  double x = minn(sen1, sen2);
  x = minn(x, sen3);
  return x;
}

double min_val4(double sen1, double sen2, double sen3, double sen4) {
  double x = minn(sen1, sen2);
  x = minn(x, sen3);
  x = minn(x, sen4);
  return x;
}

double minn(double a, double b) {
  double data = a;
  if (data > b) { data = b; }
  return data;
}