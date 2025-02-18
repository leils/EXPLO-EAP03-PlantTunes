#include <Adafruit_NeoPixel.h>

#define CAP_POWER_PIN_1 10
#define CAP_OUT_PIN_1 11
#define CAP_POWER_PIN_2 8
#define CAP_OUT_PIN_2 9
#define BUTTON_PIN 12


#define PIXEL_PIN 6 
#define NUMPIXELS 30 
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(CAP_POWER_PIN_1, OUTPUT);
  pinMode(CAP_OUT_PIN_1, INPUT_PULLDOWN);  
  pinMode(CAP_POWER_PIN_2, OUTPUT);
  pinMode(CAP_OUT_PIN_2, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  digitalWrite(CAP_POWER_PIN_1, HIGH);
  digitalWrite(CAP_POWER_PIN_2, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  byte buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    Serial.println("Button registered, rebooting sensor");
    digitalWrite(CAP_POWER_PIN_1, LOW);
    digitalWrite(CAP_POWER_PIN_2, LOW);
    digitalWrite(LED_BUILTIN, HIGH);

    delay(1000);
    digitalWrite(CAP_POWER_PIN_1, HIGH);
    digitalWrite(CAP_POWER_PIN_2, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }

  byte cap1 = digitalRead(CAP_OUT_PIN_1);
  byte cap2 = digitalRead(CAP_OUT_PIN_2);

  Serial.print(cap1);
  Serial.print(" ");
  Serial.println(cap2);

  if (cap1 && !cap2) {
    pixels.clear();
    for(int i=NUMPIXELS-1; i>17; i--) {
      pixels.setPixelColor(i, pixels.Color(100,100,0));
    }
  } else if (!cap1 && cap2) {
    pixels.clear();
    for (int i=0; i<12; i++) {
      pixels.setPixelColor(i, pixels.Color(100,100,0));
    }
  } else if (cap1 && cap2) {
    pixels.fill(pixels.Color(100,100,100));

   
  } else {
    pixels.clear(); // Set all pixel colors to 'off'
  }
  pixels.show();
  
  delay(50);
}
