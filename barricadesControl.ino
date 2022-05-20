#include <Servo.h>
#include "stateMachine.h"

const int BARRICADE_PIN = 9;
const int BARRICADE_MAX_ANGLE = 180;
const int BARRICADE_MIN_ANGLE = 0;

Servo servo;

int currentServoAngle;
boolean barricadesOpened;

void barricadesControlSetup() {
  currentServoAngle = BARRICADE_MAX_ANGLE;
  barricadesOpened = false;
  servo.attach(BARRICADE_PIN);
  servo.write(currentServoAngle);
}

void setServoAngle(int changeAmount) {
  currentServoAngle += changeAmount;
  servo.write(currentServoAngle);
}

const int BARRICADE_INTERVAL = 150;
unsigned long prevMillisBarricades;
void entryBarricadesClosing(unsigned long curMillisBarricades) {
  prevMillisBarricades = curMillisBarricades;
}

void doBarricadesClosing(unsigned long curMillisBarricades) {

  doOrangeFlicker(curMillisBarricades);
  speakerBuzz(curMillisBarricades, false);
  if (curMillisBarricades - prevMillisBarricades >= BARRICADE_INTERVAL) {
    if (currentServoAngle < BARRICADE_MAX_ANGLE) {
      setServoAngle(2);
    } else {
      barricadesOpened = false;
    }
    prevMillisBarricades = curMillisBarricades;
  }
}

void entryBarricadesClosed() {
}

void exitBarricadesClosed() {
}

void entryBarricadesOpening(unsigned long curMillisBarricades) {
  prevMillisBarricades = curMillisBarricades;
}

void doBarricadesOpening(unsigned long curMillisBarricades) {
  
  doOrangeFlicker(curMillisBarricades);
  if (curMillisBarricades - prevMillisBarricades >= BARRICADE_INTERVAL) {
    if (currentServoAngle > BARRICADE_MIN_ANGLE) {
      setServoAngle(-2);
    } else {
      barricadesOpened = true;
    }
    prevMillisBarricades = curMillisBarricades;
  }
}

void entryBarricadesOpened() {
  showCountdownDisplay(millis());
}

void exitBarricadesOpened() {
  exitSpeakerBuzz();
  exitCountdownDisplay();
  setEmergency(false);
  resetTrainControl();
  resetLightPriority();
}

boolean getBarricadesOpened() {
  return barricadesOpened;
}

void exitBarricadesClosing() {
  exitSpeakerBuzz();  
}

void doBarricadesClosed(unsigned long mills) {
  doOrangeFlicker(mills);  
}

void exitBarricadesOpening() {
  
}

void doBarricadesOpened() {
  countdownDisplay(millis());
  doOrangeFlicker(millis());
  speakerBuzz(millis(), true);  
}
