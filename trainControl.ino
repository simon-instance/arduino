# include "stateMachine.h"

const int EAST_PIN = 4;
const int WEST_PIN = 11;

boolean trainIncoming;
boolean trainCrossed;
int checkCrossedPin;

void trainControlSetup() {
  trainIncoming = false;
  trainCrossed = true;
  pinMode(EAST_PIN, INPUT);
  pinMode(WEST_PIN, INPUT);
}

void resetTrainControl() {
  trainIncoming = false;
  trainCrossed = true;  
}

void setTrainEmergency() {
  trainIncoming = true;
  trainCrossed = false;  
}

boolean checkTrainIncoming() {
  if (digitalRead(EAST_PIN) == 1) {
    checkCrossedPin = WEST_PIN;
    return true;
  } else if (digitalRead(WEST_PIN) == 1) {
    checkCrossedPin = EAST_PIN;
    return true;  
  }
  return false;
}

boolean checkTrainCrossed() {
  if(digitalRead(checkCrossedPin) == 1) {
    return true;
  }  
  return false;
}

void exitBothRed() {
  trainIncoming = true;
  trainCrossed = false;
  turnOffRedLights();
}
