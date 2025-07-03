#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A, &Wire1)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove any trailing \r or whitespace

    Serial.print("You said: ");
    Serial.println(input);

    if (input == "recal") {
      Serial.println("Recalibrating to ambient");
      cap.begin(0x5A, &Wire1);
    } else if (input == "baseline") {
      Serial.println(cap.baselineData(0));
    }
  }

  else {
    Serial.println(cap.baselineData(0));

    delay(50);
  }
}
