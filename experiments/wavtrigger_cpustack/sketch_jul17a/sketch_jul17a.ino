#include <wavTrigger.h>

wavTrigger wTrig;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial1.begin(9600);

  delay(2000);

  wTrig.start();
  delay(20);

  wTrig.stopAllTracks();
  delay(20);

  wTrig.trackLoop(1, true);
  wTrig.trackGain(1, 0);
  delay(20);
  wTrig.trackPlaySolo(1);

}

void loadAndPlayTracksMuted() {
  // Mark all tracks for looping, mute, and load them
  for (int i=0; i < 4; i++ ){
    wTrig.trackLoop(i, true);
    delay(10);    // The Wav Trigger can't handle consecutive commands via serial, needs some processing time 
    wTrig.trackGain(i, -70);
    delay(10);
    wTrig.trackLoad(i);
    delay(10);
   }

   wTrig.resumeAllInSync(); //Play all tracks
}

void loop() {
  //wTrig.trackPlaySolo(1);
  //delay(5000);
}
