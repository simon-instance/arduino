const int NORTH_PIN = 6;
const int SOUTH_PIN = 7;

const int NORTH = 0;
const int SOUTH = 1;

int lightPriority;
boolean firstPress;
void trafficLightButtonsSetup() {
  pinMode(NORTH_PIN, INPUT);
  pinMode(SOUTH_PIN, INPUT);
  lightPriority = 3;
  firstPress = true;
}

int getLightPriority() {
  return lightPriority;
}
void resetLightPriority() {
  lightPriority = 3;
}

unsigned long doubleTimer;

void setDoubleTimer() {
  doubleTimer = millis();
}

unsigned long getDoubleTimer() {
  return doubleTimer;
}

boolean doublePressed = false;

void setDoublePress(boolean newDoublePressed) {
  doublePressed = newDoublePressed;
}

void resetDoublePress() {
  doublePressed = false;
}

boolean getDoublePress() {
  return doublePressed;
}

const int DOUBLE_PRESS_PERIOD = 2000;
const int DOUBLE_PRESS_COOLDOWN = 500;
int lastPressed = 2;

void resetPressed() {
  lastPressed = 2;
  setDoublePress(false);
  firstPress = true;
}
void checkWaiting() {
  if ((digitalRead(NORTH_PIN) == 1 || digitalRead(SOUTH_PIN) == 1) && firstPress == true) {
    if(digitalRead(NORTH_PIN) == 1) {
      lastPressed = NORTH;  
    } else {
      lastPressed = SOUTH;  
    }
    setDoubleTimer();
    firstPress = false;
  }

  if (millis() >= getDoubleTimer() + DOUBLE_PRESS_COOLDOWN && millis() <= getDoubleTimer() + DOUBLE_PRESS_PERIOD && firstPress == false) {
    if (digitalRead(NORTH_PIN) == 1) {
      lastPressed = NORTH;
      setDoublePress(true);
    } else if (digitalRead(SOUTH_PIN) == 1) {
      lastPressed = SOUTH;
      setDoublePress(true);
    }
  } else if (millis() > getDoubleTimer() + DOUBLE_PRESS_PERIOD && firstPress == false) {
    lightPriority = lastPressed;
  }
}
