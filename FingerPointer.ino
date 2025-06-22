#include "Wire.h"
#include <MPU6050_light.h>
#include <BleMouse.h>
#include "clickHandler.h"
#include "movementHandler.h"
#include "mouseState.h"
#include "plotMpuData.h"

BleMouse bleMouse;
MPU6050 mpu(Wire);

State currentState = MOVING;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  bleMouse.begin();

  
  while (!bleMouse.isConnected()) {
    delay(100);
  }

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  if (status != 0) {
    while (1); // halt on error
  }

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true);
  Serial.println("Done.\n");
}

void loop() {
  mpu.update();
  handleMovement(-mpu.getGyroX(), -mpu.getGyroZ());
  handleClick(mpu.getAccX(), mpu.getAccZ());
  
  delay(10); 
}