#include "stateMachine.h"

AppState appState;

void stateMachineSetup() {
  appState = BOTH_RED;
}

// program setup
void setup() {
  Serial.begin(9600);
  trafficLightSetup();
  trafficLightButtonsSetup();
  barricadesControlSetup();
  trainControlSetup();
  setupDisplay();
  setupSpeaker();
}

// millis set every time the loop runs
unsigned long prevMillis;

// while loop for the program
void loop() {
  unsigned long curMillis = millis();

  switch (appState) {
    case BOTH_RED:
      doBothRed();

      // getEmergency() returns true when an incoming train has been detected
      // getLightPriority() returns 0 when the current priority is NORTH, it returns 1 when the current priority is SOUTH
      if ((checkTrainIncoming() && !checkTrainCrossed()) || getEmergency() == true) {
        exitBothRed();
        appState = BARRICADES_CLOSING;
        entryBarricadesClosing(curMillis);
      } else if (getLightPriority() == 0 && !checkTrainIncoming()) {
        exitBothRed();
        appState = BARRICADES_CLOSING;
        entryGreen(0, curMillis);
      } else if (getLightPriority() == 1 && !checkTrainIncoming()) {
        exitBothRed();
        appState = BARRICADES_CLOSING;
        entryGreen(1, curMillis);
      }
      setEmergency(false);
      break;
    case NORTH_GREEN:
      // if the cooldown (6 seconds) has elapsed, the if will be true if also a train hasn't been detected crossing
      if (checkGreenLightTimeElapsed(curMillis)) {
        exitGreen();
        entryOrange(curMillis);
        appState = NORTH_ORANGE;
      }
      if (checkTrainIncoming() && !checkTrainCrossed()) {
        exitGreen();
        entryEmergencyOrange(curMillis);
        appState = NORTH_ORANGE;
      }
      break;
    case SOUTH_GREEN:
      // if the cooldown (6 seconds) has elapsed, the if will be true if also a train hasn't been detected crossing
      if (checkGreenLightTimeElapsed(curMillis)) {
        exitGreen();
        entryOrange(curMillis);
        appState = SOUTH_ORANGE;
      }
      if (checkTrainIncoming() && !checkTrainCrossed()) {
        exitGreen();
        entryEmergencyOrange(curMillis);
        appState = SOUTH_ORANGE;
      }
      break;
    case NORTH_ORANGE:
      if(getEmergency()) {
          if(checkOrangeLightTimeElapsed(curMillis)) {
            appState = BARRICADES_CLOSING;
          }
      } else if (checkOrangeLightTimeElapsed(curMillis)) {
        exitOrange(curMillis);
        entryClearance(curMillis);
        appState = CLEARANCE;
      }
      if (checkTrainIncoming() && !checkTrainCrossed()) {
        entryEmergencyOrange(curMillis);
      }
      break;
    case SOUTH_ORANGE:
      if(getEmergency()) {
          if(checkOrangeLightTimeElapsed(curMillis)) {
            appState = BARRICADES_CLOSING;
          }
      } else if (checkOrangeLightTimeElapsed(curMillis)) {
        exitOrange(curMillis);
        entryClearance(curMillis);
        appState = CLEARANCE;
      }
      if (checkTrainIncoming() && !checkTrainCrossed()) {
        entryEmergencyOrange(curMillis);
      }
      break;
    case BARRICADES_CLOSING:
      doBarricadesClosing(curMillis);
      if (!getBarricadesOpened()) {
        exitBarricadesClosing();
        appState = BARRICADES_CLOSED;
        entryBarricadesClosed();
      }
      break;
    case BARRICADES_CLOSED:
      doBarricadesClosed(curMillis);
      if (checkTrainCrossed()) {
        exitBarricadesClosed();
        appState = BARRICADES_OPENING;
        entryBarricadesOpening(curMillis);
      }
      break;
    case BARRICADES_OPENING:
      doBarricadesOpening(curMillis);
      if (getBarricadesOpened()) {
        exitBarricadesOpening();
        appState = BARRICADES_OPENED;
        entryBarricadesOpened();
      }
      break;
    case BARRICADES_OPENED:
      doBarricadesOpened();
      if (getCountdownArrayKeyCounter() == 7) {
        exitBarricadesOpened();
        appState = BOTH_RED;
        entryBothRed();
      }
      break;
    case CLEARANCE:
      if(checkClearanceTimeElapsed(curMillis)) {
        exitClearance();
        appState = BOTH_RED;
        entryBothRed();  
      }
      if (checkTrainIncoming() && !checkTrainCrossed()) {
        entryEmergencyOrange(curMillis);
      }
      break;
  }
  prevMillis = curMillis;
  showLeds();
}
