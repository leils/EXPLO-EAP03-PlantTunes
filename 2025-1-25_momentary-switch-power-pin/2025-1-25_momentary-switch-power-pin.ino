#include <FastLED.h> 

#define CAP_POWER_PIN_1 10
#define CAP_OUT_PIN_1 11
#define CAP_POWER_PIN_2 8
#define CAP_OUT_PIN_2 9
#define BUTTON_PIN 12


#define NUM_LEDS 1
#define DATA_PIN 5
#define CLOCK_PIN 6
CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(9600);
  pinMode(CAP_POWER_PIN_1, OUTPUT);
  pinMode(CAP_OUT_PIN_1, INPUT_PULLDOWN);  
  pinMode(CAP_POWER_PIN_2, OUTPUT);
  pinMode(CAP_OUT_PIN_2, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical

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
    leds[0] = CRGB::Yellow;
  } else if (!cap1 && cap2) {
    leds[0] = CRGB::Blue;
  } else if (cap1 && cap2) {
    leds[0] = CRGB::Green;
  } else {
    leds[0] = CRGB::Black;
  }

  FastLED.show();
  
  delay(50);
}
