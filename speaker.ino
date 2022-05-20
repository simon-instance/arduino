const int NOTE_E6 = 1319;
const int SPEAKER_PIN = 10;

void setupSpeaker()
{
  pinMode(SPEAKER_PIN, OUTPUT);
}

const int SPEAKER_INTERVAL = 500;
int counter;

unsigned long prevMillisSpeaker;
void entrySpeakerBuzz(unsigned long curMillis) {
  prevMillisSpeaker = curMillis;
  counter = 0;
}


void speakerBuzz(unsigned long curMillis, boolean opening)
{
//  counter++;
//  if (curMillis - prevMillisSpeaker >= SPEAKER_INTERVAL 
//    && (opening ? counter < 4 : true)) {
//    tone(SPEAKER_PIN, NOTE_E6);
//  }
//  if (curMillis - prevMillisSpeaker >= SPEAKER_INTERVAL * 2) {
//    noTone(SPEAKER_PIN);
//    prevMillisSpeaker = curMillis;
//  }
}

void exitSpeakerBuzz() {
  counter = 0;
  noTone(SPEAKER_PIN);
}
