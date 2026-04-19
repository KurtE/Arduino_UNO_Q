#include <Arduino_Modulino.h>
#include <Arduino_LTR381RGB.h>

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  if (!RGB.begin()) {
    Serial.println("Failed to initialize rgb sensor!");
    while (1);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500);
}
