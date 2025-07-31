#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <wavTrigger.h>

#define delayMs 25 
bool triggerOn = false;
/* ------------------ WavTrigger Variables ---------------------- */
/*
    Note: On the Uno R4, the WavTrigger communicates over Serial1.
    WavTrigger.h needs to be updated to use Serial1
*/
wavTrigger wTrig;
#define MINVOLUME -31
#define MAXVOLUME 4 

/* ------------------ MPR121 Variables ---------------------- */
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

#define safetyZone 20
int presenceThreshold = 300; // Default value, should be updated based on calibration
int lowestThreshold = 300;

const int numReadings = 16;

int readings[numReadings];  // the readings from the input
int readIndex = 0;          // the index of the current reading
int readTotal = 0;              // the running total
int readAverage = 0;            // the average

int lastReadAverage = 0;

/* ------------------ LEDs Variables ---------------------- */
#define LEDS_PIN A5
#define NUMPIXELS 7

Adafruit_NeoPixel pixels(NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup()
{
    Serial.begin(9600);

    delay(2000);

    /* ------------------ WavTrigger Setup ---------------------- */
    /*
        The wavTrigger has an interesting quirk where its buffer can't
        handle sequential serial commands. We need to wait a few ms
        between sending commands to ensure they get read.
    */
    wTrig.start();
    delay(20);

    wTrig.stopAllTracks();
    delay(20);

    wTrig.trackLoop(1, true);
    delay(20);

    wTrig.masterGain(-70);
    delay(20);
    wTrig.trackPlaySolo(1);

    /* ------------------ MPR121 Setup ---------------------- */

    Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

    if (!cap.begin(0x5A, &Wire1))
    {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            ;
    }

    Serial.println("MPR121 found!");

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        readings[thisReading] = 0;
    }

    /* ------------------ LED Setup ---------------------- */

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear(); // Set all pixel colors to 'off'

    /* ------------------ Calibration Setup ---------------------- */

    calibrateAverage();
}

void calibrateAverage() {
    Serial.println("------------ Calibration, do not touch the sensor!");
    pixels.fill(pixels.Color(255, 0, 0));
    pixels.show();

    delay(2000); // Take the baseline of the last 2 seconds as our presenceThreshold
    presenceThreshold = cap.baselineData(0) - safetyZone;    // This is probably too high ... will lead to flaky data
    lowestThreshold = presenceThreshold;

    // Fill the read buffer with real readings
    for (int i=0; i < numReadings; i++) {
        takeCapReading();
        delay(delayMs);
    }

    Serial.println("Lows Calibration, please touch the sensor.");
    pixels.fill(pixels.Color(0, 255, 0));
    pixels.show();

    for (int i=0; i < 300; i++) {
        takeCapReading();
        if (readAverage < lowestThreshold) {    // Find the lowest average reading over this time 
            lowestThreshold = readAverage;
        }
        delay(delayMs);
    }

    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();

    lastReadAverage = readAverage;
    // Serial.print("presenceThershold: ");
    // Serial.print(presenceThreshold);
    // Serial.print(" lowestThreshold: ");
    // Serial.println(lowestThreshold);

}

void loop()
{
    takeCapReading();
    Serial.println(readAverage);

    if (readAverage < (presenceThreshold)) {
        int mappedVol = int(map(readAverage, lowestThreshold, presenceThreshold, MAXVOLUME, MINVOLUME));
        wTrig.masterGain(mappedVol);

        pixels.clear();
        int mappedLightIndex = int(map(readAverage, lowestThreshold, presenceThreshold, 6, 0));
        for (int i=0; i <= mappedLightIndex; i++) {
            pixels.setPixelColor(i, pixels.Color(100, 100, 30));
        }

        pixels.show();
        triggerOn = true;
    }
    else if ((readAverage > presenceThreshold) && (triggerOn)) // only set volume if the trigger is already on
    { // Rising edge, touch lost
        wTrig.masterGain(-70);
        pixels.clear();
        pixels.show();

        triggerOn = false;
    }

    delay(delayMs);
}

void takeCapReading() {
    readTotal = readTotal - readings[readIndex];
    readings[readIndex] =  cap.filteredData(0); // read from the sensor:
    readTotal = readTotal + readings[readIndex]; // add the reading to the total:
    readIndex = readIndex + 1; // advance to the next position in the array:

    if (readIndex >= numReadings) { // if we're at the end of the array...
        readIndex = 0; // ...wrap around to the beginning:
    }

    readAverage = readTotal / numReadings; // calculate the average:
}
