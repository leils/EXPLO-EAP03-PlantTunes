/*************************************************** 
  Plant Tunes - Single Channel
  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

 ****************************************************/
 
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_CAP1188.h"

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// CS pin is used for software or hardware SPI
#define CAP1188_CS  10

// These are defined for software SPI, for hardware SPI, check your 
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13

// Or...Use I2C, with reset pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

// Storage for last 5 readings of CAP pin 0 
const int bufferSize = 5; // Size of the circular buffer
int sensorReadings[bufferSize]; // Array to store the readings
int currentIndex = 0; // Index to track the current position in the buffer

void setup() {
  Serial.begin(9600);

  int cnt = 1000;     // Will wait for up to ~1 second for Serial to connect.
  while (!Serial && cnt--) {
    delay(1);
  }
  Serial.println("CAP1188 test!");

  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

  // Write sensitivity from 0x7f(1x) to 0x2f(32x, default)
  // Referenced from https://forums.adafruit.com/viewtopic.php?t=55900 
  cap.writeRegister(CAP1188_SENSITIVITY, 0x6F);  // 4x  sensitivity
  Serial.print("Sensitivity: 0x");
  Serial.println(cap.readRegister(CAP1188_SENSITIVITY), HEX);

  // Init all sensor readings to 0 
  for (int i = 0; i < bufferSize; i++) {
    sensorReadings[i] = 0;
  }
}

void loop() {
  uint8_t touched = cap.touched();

  if (touched & (1 << 7)) {
    Serial.print("Sensitivity: 0x");
    Serial.println(cap.readRegister(CAP1188_SENSITIVITY), HEX);
  }
  // Add the new reading to the buffer
  addReading(touched);
  reportStableState();
  delay(50);
}

// Function to add a new reading using FIFO logic
void addReading(int newValue) {
  // Shift all readings one position to the left
  for (int i = 0; i < bufferSize - 1; i++) {
    sensorReadings[i] = sensorReadings[i + 1];
  }
  // Add the new value to the last position
  sensorReadings[bufferSize - 1] = newValue;
}

int reportStableState() {
  int sumTotal = 0;
  for (int i=0; i<5; i++) {
    sumTotal += sensorReadings[i];
  }
  if (sumTotal == 0) {
    Serial.println(0);
  } else if (sumTotal == 5) {
    Serial.println(1);
  } 
}

