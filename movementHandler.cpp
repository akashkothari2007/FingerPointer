#include "movementHandler.h"

float smoothedGyroX = 0;
float smoothedGyroZ = 0;

float alpha = 0.8;
float gain = 0.02;

float deadZone = 1.5;
float movingDeadzone = 1.5;
float pausedDeadzone = 30;

float timeForStillness = 200;
unsigned long stillTime;
bool stopped = false;

void handleMovement(float gyroX, float gyroZ) {
  smoothedGyroX = alpha * smoothedGyroX + (1 - alpha) * gyroX;
  smoothedGyroZ = alpha * smoothedGyroZ + (1 - alpha) * gyroZ;

  if (currentState == MOVING) {
    deadZone = movingDeadzone;
  } else {
    deadZone = pausedDeadzone;
  }



  if (abs(smoothedGyroX) < deadZone) smoothedGyroX = 0;
  if (abs(smoothedGyroZ) < deadZone) smoothedGyroZ = 0;



  if (smoothedGyroX == 0 && smoothedGyroZ == 0) {
    if (!stopped) {
      stillTime = millis();
      stopped = true;
    }

    if (millis()-stillTime >= timeForStillness && stopped) {
      if (currentState!= CLICKING) currentState = WAITING_FOR_GESTURE;
    }
  } else {
    currentState = MOVING;
    stopped = false;
  }

  float moveX = constrain(-gain * smoothedGyroZ * abs(smoothedGyroZ), -20, 20);
  float moveY = constrain(gain * smoothedGyroX * abs(smoothedGyroX), -20, 20);
  if (currentState != CLICKING) {
    bleMouse.move((int)moveX, (int)moveY);
  }
}