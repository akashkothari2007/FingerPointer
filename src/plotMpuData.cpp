#include "plotMpuData.h"

void plotData(float a) {
  Serial.println(a);
}

void plotData(float a, float b) {
  Serial.print(a);
  Serial.print(" ");
  Serial.println(b);
}

void plotData(float a, float b, float c) {
  Serial.print(a);
  Serial.print(" ");
  Serial.print(b);
  Serial.print(" ");
  Serial.println(c);
}