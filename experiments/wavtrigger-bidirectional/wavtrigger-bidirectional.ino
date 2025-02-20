// ****************************************************************************
//       Sketch: WTrigAdvanced - Wav Trigger Advanced Example
// Date Created: 11/7/2016
//
//     Comments: Demonstrates advanced, two-way, serial control of the WAV
//               Trigger from an Arduino. Requires WAV Trigger firmware v1.30
//               or above.
//
//  Programmers: Jamie Robertson, info@robertsonics.com
//
// ****************************************************************************
//
// To use this sketch with an UNO, you'll need to:
//
// 1) Download and install the AltSoftSerial library.
// 2) Download and install the Metro library.
// 3) Connect 3 wires from the UNO to the WAV Trigger's serial connector:
//
//    Uno           WAV Trigger
//    ===           ===========
//    GND  <------> GND
//    Pin9 <------> RX
//    Pin8 <------> TX
//
//    If you want to power the WAV Trigger from the Uno, then close the 5V
//    solder jumper on the WAV Trigger and connect a 4th wire:
//
//    5V   <------> 5V
//
//    (If you are using an Arduino with extra hardware serial ports, such as
//    an Arduino Mega or Teensy, you don't need AltSoftSerial, and you should
//    edit the wavTrigger.h library file to select the desired serial port
//    according to the documentation contained in that file. And use the
//    appropriate TX/RX pin connections to your Arduino)
//
// 4) Download and install the demo wav files onto the WAV Trigger's microSD
//    card. You can find them here:
//
//    http://robertsonics.com/2015/04/25/arduino-serial-control-tutorial/
//
//    You can certainly use your own tracks instead, although the demo may
//    not make as much sense. If you do, make sure your tracks are at least
//    10 to 20 seconds long and have no silence at the start of the file.

#include <wavTrigger.h>

#define LED 13                // our LED

wavTrigger wTrig;             // Our WAV Trigger object

byte gLedState = 0;           // LED State
int  gSeqState = 0;           // Main program sequencer state
int  gRateOffset = 0;         // WAV Trigger sample-rate offset
int  gNumTracks;              // Number of tracks on SD card

char gWTrigVersion[VERSION_STRING_LEN];    // WAV Trigger version string


// ****************************************************************************
void setup() {
  
  // Serial monitor
  Serial.begin(9600);

  // If the Arduino is powering the WAV Trigger, we should wait for the WAV
  //  Trigger to finish reset before trying to send commands.
  delay(1000);

  // WAV Trigger startup at 57600
  wTrig.start();
  delay(10);
  
  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the WAV Trigger was already playing.
  wTrig.stopAllTracks();
  wTrig.samplerateOffset(0);
  
  // Enable track reporting from the WAV Trigger
  wTrig.setReporting(true);
  
  // Allow time for the WAV Trigger to respond with the version string and
  //  number of tracks.
  delay(100); 
  
  // If bi-directional communication is wired up, then we should by now be able
  //  to fetch the version string and number of tracks on the SD card.
  if (wTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
      Serial.print(gWTrigVersion);
      Serial.print("\n");
      gNumTracks = wTrig.getNumTracks();
      Serial.print("Number of tracks = ");
      Serial.print(gNumTracks);
      Serial.print("\n");
  }
  else
      Serial.print("WAV Trigger response not available");
}


void loop() {
  
  // Call update on the WAV Trigger to keep the track playing status current.
  wTrig.update();
  
  // Delay 30 msecs
  delay(30);
}

