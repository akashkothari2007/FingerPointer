#ifndef SHARED_STATE_H
#define SHARED_STATE_H

enum State {
  MOVING,
  CLICKING,
  WAITING_FOR_GESTURE
};

extern State currentState;

#endif