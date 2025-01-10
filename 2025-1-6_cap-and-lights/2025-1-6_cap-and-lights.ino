/***************************************************
  Plant Tunes - Single Channel
  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

 ****************************************************/

#include <Wire.h>
#include <SPI.h>
#include "Adafruit_CAP1188.h"
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 5
#define CLOCK_PIN 6

CRGB leds[NUM_LEDS];

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET 9

// CS pin is used for software or hardware SPI
#define CAP1188_CS 10

// These are defined for software SPI, for hardware SPI, check your
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI 11
#define CAP1188_MISO 12
#define CAP1188_CLK 13

// Or...Use I2C, with reset pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

const int NumCapPins = 8;

// Storage for last 5 readings of CAP pin 0
const int bufferSize = 5;                   // Size of the buffer
int sensorReadings[NumCapPins][bufferSize]; // Array to store the last 5 readings of each pin
int stableReadings[NumCapPins] = { 0 };

#define SWITCH_PIN 2
int lastSwitchRead = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical

  int cnt = 1000; // Will wait for up to ~1 second for Serial to connect.
  while (!Serial && cnt--)
  {
    delay(1);
  }
  Serial.println("CAP1188 test!");

  if (!cap.begin())
  {
    Serial.println("CAP1188 not found");
    while (1)
      ;
  }
  Serial.println("CAP1188 found!");

  // Write sensitivity from 0x7f(1x) to 0x2f(32x, default)
  // Referenced from https://forums.adafruit.com/viewtopic.php?t=55900
  cap.writeRegister(CAP1188_SENSITIVITY, 0x3F); 
  Serial.print("Sensitivity: 0x");
  Serial.println(cap.readRegister(CAP1188_SENSITIVITY), HEX);

  // Init all sensor readings to 0
  for (int p = 0; p < NumCapPins; p++)
  {
    for (int i = 0; i < bufferSize; i++)
    {
      sensorReadings[p][i] = 0;
    }
  }
}

void loop()
{
  uint8_t touched = cap.touched();

  if (touched & (1 << 7))
  {
    Serial.print("Sensitivity: 0x");
    Serial.println(cap.readRegister(CAP1188_SENSITIVITY), HEX);
  }

  addReadings(touched); // Add the new reading to the buffer
  updateStableState();  // Debounce readings
  handleButton();



  // Report current stable state via Serial
  for (int i=0; i<NumCapPins; i++) {
    Serial.print(stableReadings[i]); Serial.print(" ");
  }
  Serial.println();

  handleLight();

  delay(50);
}


// For each pin, adds the current sensor reading to the end of the readings buffer
void addReadings(uint8_t touchedReadings)
{
  // Shift all readings one position to the left
  for (int p = 0; p < NumCapPins; p++)
  {
    for (int i = 0; i < bufferSize - 1; i++)
    {
      sensorReadings[p][i] = sensorReadings[p][i + 1];
    }

    bool currentSensorTouched = (touchedReadings & (1 << p));
    sensorReadings[p][bufferSize - 1] = currentSensorTouched;
  }
}

void updateStableState() {
  for (int pin = 0; pin < NumCapPins; pin++) {
    int readTotalForCurrentPin = 0;
    for (int r = 0; r < bufferSize; r++) {
      readTotalForCurrentPin += sensorReadings[pin][r];
    }
    if (readTotalForCurrentPin == 0) {  // pin has been low 
      stableReadings[pin] = 0;
    } else if (readTotalForCurrentPin == 5) {
      stableReadings[pin] = 1;
    }
  }
}

void handleLight() {
  if (stableReadings[1]) {
    leds[0] = CRGB::Green;
  } else {
    leds[0] = CRGB::Black;
  }
  FastLED.show();
}

void handleButton() {
  byte switchState = digitalRead(SWITCH_PIN);  //read the state of the input pin (HIGH or LOW)
  if (switchState == LOW && lastSwitchRead == HIGH) {
    cap.writeRegister(CAP1188_SENSITIVITY, 0x3F);
  } else if (switchState == HIGH && lastSwitchRead == LOW) {
    cap.writeRegister(CAP1188_SENSITIVITY, 0x6F);
  }
  lastSwitchRead = switchState;
}