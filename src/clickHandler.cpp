#include "clickHandler.h"

float lastAccX = 0;
float lastAccY = 0;
unsigned long timeAtClick;
float clickCoolDown = 150;
bool clicking = false;

void handleClick(float accX, float accY) {
  float jerkX = accX - lastAccX;
  float jerkY = accY - lastAccY;
  Serial.print(jerkX);
  Serial.print(" ");
  Serial.print(jerkY);
  Serial.print(" ");

  lastAccX = accX;
  lastAccY = accY;
  if ((jerkX > 0.25 || jerkY > 0.1) && !clicking && currentState == WAITING_FOR_GESTURE) {
    clicking = true;
    currentState = CLICKING;
    timeAtClick = millis();
    bleMouse.click(MOUSE_LEFT);
  } else if (currentState == MOVING && !clicking && jerkX > 0.25) {
    clicking = true;
    currentState = CLICKING;
    timeAtClick = millis();
    bleMouse.click(MOUSE_LEFT);
  }

  if (clicking && millis() - timeAtClick >= clickCoolDown) {
    if (currentState != MOVING) {
      currentState = WAITING_FOR_GESTURE;
    }
    clicking = false;
  }
}