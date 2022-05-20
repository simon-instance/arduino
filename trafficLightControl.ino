#include "stateMachine.h"

// Better for readability later on in the code
const int TRAFFIC_LIGHT_AMOUNT = 6;

// led pin, state to be enabled at, currently enabled
int trafficLightPins[TRAFFIC_LIGHT_AMOUNT][3] = {
  {13,  BOTH_RED, 0},
  {16,  BOTH_RED, 0},
  {12,  NORTH_ORANGE, 0},
  {8,   NORTH_GREEN, 0},
  {15,  SOUTH_ORANGE, 0},
  {14,  SOUTH_GREEN, 0}
};

void resetTrafficLights() {
  for (int i = 0; i < TRAFFIC_LIGHT_AMOUNT; i++) {
    if (trafficLightPins[i][1] == BOTH_RED) {
      trafficLightPins[i][2] = true;
    } else {
      trafficLightPins[i][2] = false;
    }
  }
}

void entryBothRed() {
  resetTrafficLights();
  appState = BOTH_RED;
}

void turnOffRedLights() {
  trafficLightPins[0][2] = false;
  trafficLightPins[1][2] = false;
}

void trafficLightSetup() {
  for (int i = 0; i < TRAFFIC_LIGHT_AMOUNT; i++) {
    pinMode(trafficLightPins[i][0], OUTPUT);
  }
}

int* getLights() {
  int lights[2];
  int lightsArrayCounter = 0;
  for (int i = 0; i < TRAFFIC_LIGHT_AMOUNT; i++) {
    if (trafficLightPins[i][1] == appState) {
      lights[lightsArrayCounter] = trafficLightPins[i][0];
      lightsArrayCounter++;
    }
  }
  return lights;
}

void toggleTrafficLights() {
  int *lights = getLights();
  for (int i = 0; i < TRAFFIC_LIGHT_AMOUNT; i++) {
    if (trafficLightPins[i][1] == appState) {
      trafficLightPins[i][2] = 1;
    }
  }
}

void showLeds() {
  toggleTrafficLights();
  for (int i = 0; i < TRAFFIC_LIGHT_AMOUNT; i++) {
    digitalWrite(trafficLightPins[i][0], trafficLightPins[i][2]);
  }
}

const int GREEN_LIGHT_INTERVAL = 6000;
const int ORANGE_LIGHT_INTERVAL = 3000;
unsigned long lightStart;
void entryGreen(int northOrSouth, unsigned long curMillis) {
  turnOffRedLights();
  if (northOrSouth == NORTH) {
    trafficLightPins[1][2] = true;
    appState = NORTH_GREEN;
  } else if (northOrSouth == SOUTH) {
    trafficLightPins[0][2] = true;
    appState = SOUTH_GREEN;
  }
  lightStart = curMillis;
}

void exitGreen() {
  trafficLightPins[1][2] = false;
  trafficLightPins[0][2] = false;

  resetDoublePress();
  resetPressed();
}

boolean emergency = false;
boolean getEmergency() {
  return emergency;  
}

unsigned long entryEmergencyOrangeStart;
void entryEmergencyOrange(unsigned long curMillis) {
  entryEmergencyOrangeStart = curMillis;
  if (appState == NORTH_GREEN) {
    trafficLightPins[3][2] = false;
    trafficLightPins[1][2] = true;
  }  else if (appState == SOUTH_GREEN) {
    trafficLightPins[5][2] = false;
    trafficLightPins[0][2] = true;
  }
  emergency = true;
}

void setEmergency(boolean toggle) {
  emergency = toggle;
}

const int GREEN_LIGHT_EXTRA = 3000;
boolean checkGreenLightTimeElapsed(unsigned long curMillis) {
  Serial.println(getDoublePress());
  if (curMillis - lightStart >= GREEN_LIGHT_INTERVAL + (getDoublePress() ? GREEN_LIGHT_EXTRA : 0)) {
    return true;
  }
  return false;
}

void entryOrange(unsigned long curMillis) {
  entryEmergencyOrangeStart = curMillis;
  if (appState == NORTH_GREEN) {
    trafficLightPins[3][2] = false;
    trafficLightPins[1][2] = true;
  } else if (appState == SOUTH_GREEN) {
    trafficLightPins[5][2] = false;
    trafficLightPins[0][2] = true;
  }
}

boolean checkOrangeLightTimeElapsed(unsigned long curMillis) {
  if (curMillis - entryEmergencyOrangeStart >= ORANGE_LIGHT_INTERVAL) {
    return true;
  }
  return false;
}

void exitOrange(unsigned long curMillis) {
  trafficLightPins[2][2] = false;
  trafficLightPins[4][2] = false;
  resetTrafficLights();
  lightStart = curMillis;
}

const int ORANGE_FLICKER_INTERVAL = 200;
unsigned long prevMillisOrangeFlicker;
void entryOrangeFlicker(unsigned long curMillis) {
  prevMillisOrangeFlicker = curMillis;
}

void entryClearance(unsigned long curMills) {
  appState = CLEARANCE;
  entryOrangeFlicker(curMills);
}

void doOrangeFlicker(unsigned long curMillis) {    
  trafficLightPins[0][2] = true;
  trafficLightPins[1][2] = true;
  if(curMillis - prevMillisOrangeFlicker >= ORANGE_FLICKER_INTERVAL) {
    trafficLightPins[2][2] = true;
    trafficLightPins[4][2] = true;
    if(curMillis - prevMillisOrangeFlicker >= ORANGE_FLICKER_INTERVAL * 2) {
      prevMillisOrangeFlicker = curMillis;  
    }
  } else {
    trafficLightPins[2][2] = false;
    trafficLightPins[4][2] = false;      
  }
}

const int CLEARANCE_INTERVAL = 2000;
boolean checkClearanceTimeElapsed(unsigned long curMillis) {
  if (curMillis - lightStart >= CLEARANCE_INTERVAL) {
    return true;
  }
  return false;
}

void exitClearance() {
  trafficLightButtonsSetup();
  resetTrafficLights();
  prevMillisOrangeFlicker = NULL;
}

void doBothRed() {
  // checks who pressed the button first
  checkWaiting();
}
