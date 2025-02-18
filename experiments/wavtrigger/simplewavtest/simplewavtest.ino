#include <wavTrigger.h>

wavTrigger wTrig;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial1.begin(9600);

  wTrig.start();
  delay(20);

  Serial.println("should stop all tracks");
  wTrig.stopAllTracks();
  delay(10);
  wTrig.samplerateOffset(0);

  loadAndPlayTracksMuted();
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
  if (Serial.available() > 0) {  // Check if data is available to read
    char input = Serial.read();  // Read the incoming character

    if (input == '1') {
      wTrig.trackFade(1, 0, 500, false);
    } else if (input == '2'){
      wTrig.trackFade(1, -70, 500, false);
    } else if (input == '3') {
      wTrig.trackFade(2, 0, 500, false);
    } else if (input == '4') {
      wTrig.trackFade(2, -70, 500, false);
    } else if (input == '5') {
      wTrig.trackFade(3, 0, 500, false); 
    } else if (input == '6') {
      wTrig.trackFade(3, -70, 500, false);
    } else if (input == '7') { 
      wTrig.trackFade(4, 0, 500, false);
    } else if (input == '8') {
      wTrig.trackFade(4, -70, 500, false);
    }
  }
}
