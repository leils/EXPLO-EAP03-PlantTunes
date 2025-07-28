#include <Adafruit_NeoPixel.h>
#include <Wire.h>


/* ------------------ LEDs Setup ---------------------- */
#define LEDS_PIN A5 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 7 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}

void loop() {
    pixels.fill(pixels.Color(255,255,255));
    pixels.show();
    Serial.println("on");
    delay(500);
    pixels.clear();
    Serial.println("off");
    pixels.show();

    delay(500);
//   Serial.println(cap.filteredData(0));
  
//   // -------------- Handle filtered data as control
//   if (cap.filteredData(0) < 100) {
//     myDFPlayer.volume(20);  
//     pixels.fill(pixels.Color(100,100,30));
//     pixels.show();
//   } else {
//     myDFPlayer.volume(0);  
//     pixels.clear();
//     pixels.show();
//   }

//   delay(50);
}

// void printDetail(uint8_t type, int value) {
//   switch (type) {
//     case TimeOut:
//       Serial.println(F("Time Out!"));
//       break;
//     case WrongStack:
//       Serial.println(F("Stack Wrong!"));
//       break;
//     case DFPlayerCardInserted:
//       Serial.println(F("Card Inserted!"));
//       break;
//     case DFPlayerCardRemoved:
//       Serial.println(F("Card Removed!"));
//       break;
//     case DFPlayerCardOnline:
//       Serial.println(F("Card Online!"));
//       break;
//     case DFPlayerUSBInserted:
//       Serial.println("USB Inserted!");
//       break;
//     case DFPlayerUSBRemoved:
//       Serial.println("USB Removed!");
//       break;
//     case DFPlayerPlayFinished:
//       Serial.print(F("Number:"));
//       Serial.print(value);
//       Serial.println(F(" Play Finished!"));
//       break;
//     case DFPlayerError:
//       Serial.print(F("DFPlayerError:"));
//       switch (value) {
//         case Busy:
//           Serial.println(F("Card not found"));
//           break;
//         case Sleeping:
//           Serial.println(F("Sleeping"));
//           break;
//         case SerialWrongStack:
//           Serial.println(F("Get Wrong Stack"));
//           break;
//         case CheckSumNotMatch:
//           Serial.println(F("Check Sum Not Match"));
//           break;
//         case FileIndexOut:
//           Serial.println(F("File Index Out of Bound"));
//           break;
//         case FileMismatch:
//           Serial.println(F("Cannot Find File"));
//           break;
//         case Advertise:
//           Serial.println(F("In Advertise"));
//           break;
//         default:
//           break;
//       }
//       break;
//     default:
//       break;
//   }
// }

