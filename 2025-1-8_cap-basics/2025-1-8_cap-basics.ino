/***************************************************
  Plant Tunes - Single Channel
  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

 ****************************************************/

#include <Wire.h>
#include <SPI.h>
#include "Adafruit_CAP1188.h"

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET 9

// CS pin is used for software or hardware SPI
#define CAP1188_CS 10

Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

const int NumCapPins = 8;
int readings[NumCapPins]; // array to store all the readings

// // Storage for last 5 readings of CAP pin 0
// const int bufferSize = 5;                   // Size of the buffer
// int sensorReadings[NumCapPins][bufferSize]; // Array to store the last 5 readings of each pin
// int stableReadings[NumCapPins] = { 0 };

void setup()
{
  Serial.begin(9600);

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
  cap.writeRegister(CAP1188_SENSITIVITY, 0x6F); // 4x  sensitivity
  Serial.print("Sensitivity: 0x");
  Serial.println(cap.readRegister(CAP1188_SENSITIVITY), HEX);

  // // Init all sensor readings to 0
  // for (int p = 0; p < NumCapPins; p++)
  // {
  //   for (int i = 0; i < bufferSize; i++)
  //   {
  //     sensorReadings[p][i] = 0;
  //   }
  // }
}

void loop()
{
  uint8_t touched = cap.touched();

  if (touched == 0) {
    return;
  }

  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {
      Serial.print("C"); Serial.print(i+1); Serial.print("\t");
    }
  }
  Serial.println();
  // addReadings(touched); // Add the new reading to the buffer

  // // Report current stable state via Serial
  // for (int i=0; i<NumCapPins; i++) {
  //   Serial.print(readings[i]); Serial.print(" ");
  // }
  // Serial.println();

  delay(50);
}


// For each pin, adds the current sensor reading to the end of the readings buffer
void addReadings(uint8_t touchedReadings)
{
  // Shift all readings one position to the left
  for (int p = 0; p < NumCapPins; p++)
  {
    readings[p] = (touchedReadings & (1 << p));
  }
}

