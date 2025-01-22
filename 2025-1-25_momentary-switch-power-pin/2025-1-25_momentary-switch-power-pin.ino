
#define CAP_POWER_PIN_1 10
#define CAP_OUT_PIN_1 11
#define CAP_POWER_PIN_2 8
#define CAP_OUT_PIN_2 9
#define BUTTON_PIN 13

void setup() {
  Serial.begin(9600);
  pinMode(CAP_POWER_PIN_1, OUTPUT);
  pinMode(CAP_OUT_PIN_1, INPUT_PULLDOWN);  
  pinMode(CAP_POWER_PIN_2, OUTPUT);
  pinMode(CAP_OUT_PIN_2, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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

  Serial.print(digitalRead(CAP_OUT_PIN_1));
  Serial.print(" ");
  Serial.println(digitalRead(CAP_OUT_PIN_2));
  delay(50);
}
