#ifndef STATEMACHINE_H
#define STATEMACHINE_H

enum AppState {
  BOTH_RED = 0,
  NORTH_GREEN,
  NORTH_ORANGE,
  SOUTH_GREEN,
  SOUTH_ORANGE,
  CLEARANCE,
  BARRICADES_CLOSING,
  BARRICADES_CLOSED,
  BARRICADES_OPENING,
  BARRICADES_OPENED
};

extern AppState appState;

#endif // STATEMACHINE_H
