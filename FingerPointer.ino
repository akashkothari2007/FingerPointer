#include "Wire.h"
#include <MPU6050_light.h>
#include <BleMouse.h>

BleMouse bleMouse;
MPU6050 mpu(Wire);

// Smoothed gyro values
float smoothedGyroX = 0;
float smoothedGyroZ = 0;
float lastAccX = 0;
float lastAccY = 0;
// Smoothing factor (higher = smoother, slower response)
float alpha = 0.8;
float gain = 0.02;
float deadZone = 1.5;
float pausedDeadzone = 30;

bool hasGestured;
unsigned long timeAtClick;
float clickCoolDown = 300;
bool clicking = false;

float gestureDetectionBegin = 300;
unsigned long stillTime;
bool stopped = false;

enum State {
  MOVING,
  WAITING_FOR_GESTURE
};
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



  float rawAccX = mpu.getAccX();
  float rawAccY = mpu.getAccY();

  float rawGyroX = -mpu.getGyroX();  // pitch (up/down)
  float rawGyroZ = -mpu.getGyroZ();  // yaw (left/right)

  float jerkX = rawAccX - lastAccX;
  float jerkY = rawAccY - lastAccY;

  lastAccX = rawAccX;
  if (jerkX >= 0.3 && rawGyroX < -20 && !clicking) {
    clicking = true;
    hasGestured = true;
    timeAtClick = millis();
    bleMouse.click(MOUSE_LEFT);
  }
  if (clicking && millis() - timeAtClick >= clickCoolDown) {
    clicking = false;
  }

  // Raw gyro values
  

  // Low-pass smoothing
  smoothedGyroX = alpha * smoothedGyroX + (1 - alpha) * rawGyroX;
  smoothedGyroZ = alpha * smoothedGyroZ + (1 - alpha) * rawGyroZ;

  if (currentState == WAITING_FOR_GESTURE) {
    deadZone = pausedDeadzone;
  } else {
    deadZone = 1.5;
  }

  // Deadzone
  if (abs(smoothedGyroX) < deadZone) smoothedGyroX = 0;
  if (abs(smoothedGyroZ) < deadZone) smoothedGyroZ = 0;
  if (smoothedGyroX == 0 && smoothedGyroZ == 0) {
    if (!stopped) {
      stillTime = millis();
      stopped = true;
    }

    if (!hasGestured && stillTime - millis() >= gestureDetectionBegin ) {
      currentState = WAITING_FOR_GESTURE;
    } else {
      currentState = MOVING;
    }
  } else {
    currentState = MOVING;
    hasGestured = false;
    stopped = false;
  }

  // Non-linear gain
  float moveX = constrain(-gain * smoothedGyroZ * abs(smoothedGyroZ), -20, 20);
  float moveY = constrain(gain * smoothedGyroX * abs(smoothedGyroX), -20, 20);  

  if (!clicking) {
    bleMouse.move((int)moveX, (int)moveY);
  }
  delay(10);  // Adjust for responsiveness vs jitter
}