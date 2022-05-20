//Contact SH_CP
const int SH_CP = 5;
//Contact ST_CP
const int ST_CP = 3;
//Contact DS
const int DS = 2;

int countdownArrayKeyCounter;

// @param number 0 = 5, 1 = 4, 2 = 3, 3 = 2, 4 = 1, 5 = 0, 6 = reset (all off)
const byte COUNTDOWN_NUMBERS[16] = {B01100111, B01001011, B01101101, B00111101, B00010010, B01111110, 0};

void setupDisplay() {
  countdownArrayKeyCounter = 0;
  pinMode(SH_CP, OUTPUT);
  pinMode(ST_CP, OUTPUT);
  pinMode(DS, OUTPUT);
  displayNumber(6);
}

int getCountdownArrayKeyCounter() {
  return countdownArrayKeyCounter;  
}

void displayNumber(int number) {
  // ST_CP LOW to keep LEDs from changing while reading serial data
  digitalWrite(ST_CP, LOW);

  // Shift out the bits
  shiftOut(DS, SH_CP, MSBFIRST, COUNTDOWN_NUMBERS[number]);

  // ST_CP HIGH change LEDs
  digitalWrite(ST_CP, HIGH);
}

unsigned long prevMillisCountdownDisplay;
void showCountdownDisplay(unsigned long curMillisCountdownDisplay) {
  countdownArrayKeyCounter = 0;
  prevMillisCountdownDisplay = curMillisCountdownDisplay;
}

const int COUNTDOWN_INTERVAL = 1000;
void countdownDisplay(unsigned long curMillisCountdownDisplay) {
  if(curMillisCountdownDisplay - prevMillisCountdownDisplay >= COUNTDOWN_INTERVAL) {
    displayNumber(countdownArrayKeyCounter);
    countdownArrayKeyCounter++;
    prevMillisCountdownDisplay = curMillisCountdownDisplay;
  }
}

void exitCountdownDisplay() {
  displayNumber(6);  
}
