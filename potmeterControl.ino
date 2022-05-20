const int POTMETER_PIN = A3;

int getPotmeter() {
  int potmeterWaarde = analogRead(POTMETER_PIN);
  return map(potmeterWaarde, 0, 1023, 1, 3);  
}
