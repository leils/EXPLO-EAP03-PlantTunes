/***************************************************
  Plant Tunes - Single Channel
  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

 ****************************************************/


#define SWITCH_PIN 2
int lastSwitchRead = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  Serial.println(digitalRead(SWITCH_PIN));
  delay(100);
}