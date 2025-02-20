#include <Adafruit_NeoPixel.h>
#include <wavTrigger.h>

wavTrigger wTrig;

//Following is added to poll version to determine connection
char gWTrigVersion[VERSION_STRING_LEN];
int gNumTracks;  // Number of tracks on SD card
bool wTrigConnected = false;

#define CAP_POWER_PIN_1 10
#define CAP_OUT_PIN_1 11
#define CAP_POWER_PIN_2 8
#define CAP_OUT_PIN_2 9
#define BUTTON_PIN 12

#define PIXEL_PIN 6
#define NUMPIXELS 30

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_RGB + NEO_KHZ800);

byte lastCap1 = 0;
byte lastCap2 = 0;

void setup() {
  Serial.begin(9600);

  delay(2000);

  pinMode(CAP_POWER_PIN_1, OUTPUT);
  pinMode(CAP_OUT_PIN_1, INPUT_PULLDOWN);
  pinMode(CAP_POWER_PIN_2, OUTPUT);
  pinMode(CAP_OUT_PIN_2, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  digitalWrite(CAP_POWER_PIN_1, HIGH);
  digitalWrite(CAP_POWER_PIN_2, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  wTrig.start();
  delay(10);

  loadAndPlayTracksMuted();
}

void loadAndPlayTracksMuted() {
  // Create a clean slate with no playing tracks
  wTrig.stopAllTracks();
  delay(20);
  wTrig.samplerateOffset(0);
  delay(20);
  // Mark all tracks for looping, mute, and load them
  for (int i = 0; i < 4; i++) {
    wTrig.trackLoop(i, true);
    delay(20);  // The Wav Trigger can't handle consecutive commands via serial, needs some processing time
    wTrig.trackGain(i, -70);
    delay(20);
    wTrig.trackLoad(i);
    delay(20);
  }

  wTrig.resumeAllInSync();  //Play all tracks
  delay(20);
}

void loop() {
  byte buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    Serial.println("Button registered, rebooting sensor");
    digitalWrite(CAP_POWER_PIN_1, LOW);
    digitalWrite(CAP_POWER_PIN_2, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    wTrig.stopAllTracks();

    delay(1000);
    wTrig.start();
    delay(10);
    digitalWrite(CAP_POWER_PIN_1, HIGH);
    digitalWrite(CAP_POWER_PIN_2, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    loadAndPlayTracksMuted();
  }

  byte cap1 = digitalRead(CAP_OUT_PIN_1);
  byte cap2 = digitalRead(CAP_OUT_PIN_2);


  // ---------- Catch rising/falling edges on cap sensors for sound volume
  // On a rising edge, raises track volume to 0 gain (normal volume)
  // On a falling edge, lowers track volume to -70 gain (mute)
  if (cap1 > lastCap1) {  // rising edge
    Serial.println("cap1 rising");
    wTrig.trackFade(1, 0, 500, false);
    delay(10);
  } else if (cap1 < lastCap1) {  // falling edge
    Serial.println("Cap1 falling");
    wTrig.trackFade(1, -70, 500, false);
    delay(10);
  }

  if (cap2 > lastCap2) {  // rising edge
    Serial.println("Cap2 rising");
    wTrig.trackFade(2, 0, 500, false);
    delay(10);
  } else if (cap2 < lastCap2) {  // falling edge
    Serial.println("Cap2 falling");
    wTrig.trackFade(2, -70, 500, false);
    delay(10);
  }

  // ---------- LED color handling
  // If any change has occurred since the last reading, change LED colors
  if ((cap1 != lastCap1) || (cap2 != lastCap2)) {
    if (cap1 && !cap2) {
      pixels.clear();
      for (int i = NUMPIXELS - 1; i > 17; i--) {
        pixels.setPixelColor(i, pixels.Color(100, 100, 0));
      }
    } else if (!cap1 && cap2) {
      pixels.clear();
      for (int i = 0; i < 12; i++) {
        pixels.setPixelColor(i, pixels.Color(100, 100, 0));
      }
    } else if (cap1 && cap2) {
      pixels.fill(pixels.Color(100, 100, 100));


    } else {
      pixels.clear();  // Set all pixel colors to 'off'
    }
    pixels.show();
  }


  lastCap1 = cap1;
  lastCap2 = cap2;

  delay(50);
}
