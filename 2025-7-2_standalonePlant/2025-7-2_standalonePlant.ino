#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

/* ------------------ MPR121 Setup ---------------------- */
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

/* ------------------ LEDs Setup ---------------------- */
#define LEDS_PIN        7 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 7 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  if (!cap.begin(0x5A, &Wire1)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }

  Serial.println("MPR121 found!");

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}

void loop() {
  // Serial.println(cap.baselineData(0));
  Serial.println(cap.filteredData(0));
  
  if (cap.filteredData(0) < 100) {
    pixels.fill(pixels.Color(100,100,30));
    pixels.show();
  } else {
    pixels.clear();
    pixels.show();
  }

  delay(50);
}
