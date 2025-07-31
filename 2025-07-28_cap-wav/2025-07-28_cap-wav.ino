#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <wavTrigger.h>

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

int presenceThreshold = 300; // Default value, should be updated based on calibration
int lowestThreshold = 300;
int calibrationData[100];
#define wiggleRoom 10;

int lastRead;

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

    lastRead = cap.filteredData(0);

    /* ------------------ LED Setup ---------------------- */

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear(); // Set all pixel colors to 'off'

    /* ------------------ Calibration Setup ---------------------- */

    calibrate();
}

void calibrate() {
    Serial.println("------------ Calibration!");
    pixels.fill(pixels.Color(255, 0, 0));
    pixels.show();
    delay(1000); // Take the baseline of the last 1 second as our presenceThreshold
    presenceThreshold = cap.baselineData(0);    // This is probably too high ... will lead to flaky data
    lowestThreshold = presenceThreshold;


    Serial.println("Lows Calibration, please touch the sensor.");
    pixels.fill(pixels.Color(0, 255, 0));
    pixels.show();

    // take the lowest of the 100 samples 
    for (int i=0; i < 100; i++) {
        int latestRead = cap.filteredData(0);
        if (latestRead < lowestThreshold) {
            lowestThreshold = latestRead;
        }
        delay(50);
    }

    Serial.print("Presence Threshold: ");
    Serial.print(presenceThreshold);
    Serial.print(" lowestThreshold: ");
    Serial.println(lowestThreshold);

    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();
}

void loop()
{
    int currentRead = cap.filteredData(0);
    // Serial.print("Baseline: ");
    // Serial.print(cap.baselineData(0));
    // Serial.print("Filtered: ");
    Serial.println(currentRead);

    if (currentRead < (presenceThreshold - 10)) {
        int mappedVol = int(map(currentRead, lowestThreshold, presenceThreshold, MAXVOLUME, MINVOLUME));
        wTrig.masterGain(mappedVol);

        pixels.clear();
        int mappedLightIndex = int(map(currentRead, lowestThreshold, presenceThreshold, 6, 0));
        for (int i=0; i <= mappedLightIndex; i++) {
            pixels.setPixelColor(i, pixels.Color(100, 100, 30));
        }

        pixels.show();
    }
    else if (lastRead < presenceThreshold && currentRead > presenceThreshold)
    { // Rising edge, touch lost
        wTrig.masterGain(-70);
        pixels.clear();
        pixels.show();
    }

    lastRead = currentRead;
    delay(100);
}
