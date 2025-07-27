#ifndef CLICK_HANDLER_H
#define CLICK_HANDLER_H
#include <Arduino.h>
#include <BleMouse.h>
#include "mouseState.h"

//variables

extern BleMouse bleMouse;
extern State currentState;


//Click Function
void handleClick(float accX, float accY);

#endif
