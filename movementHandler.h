#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H
#include <Arduino.h>
#include <BleMouse.h>
#include "mouseState.h"

extern BleMouse bleMouse;
extern State currentState;

void handleMovement(float gyroX, float gyroZ);

#endif