
#include <digitalWriteFast_zephyr.h>
#ifdef ARDUINO_UNO_Q
#define PIN 2
#define PIN_MARKER 3
#define PIN_NAME PB_3
#else
//Giga
#define PIN 2
#define PIN_MARKER 3
#define PIN_NAME PA_3

#endif

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nTest");
  while (!Serial && millis() < 5000)
    ;

  //pinMode(PIN, OUTPUT);
  pinMode(PIN_NAME, OUTPUT);
  pinMode(PIN_MARKER, OUTPUT);
}


void do_digitalWrite() {

  digitalWrite(PIN_MARKER, HIGH);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalWrite(PIN, HIGH);
    digitalWrite(PIN, LOW);
  }
  uint32_t delta_time = micros() - start_time;
  digitalWrite(PIN_MARKER, LOW);
  Serial.print("digitalWrite: ");
  Serial.println(delta_time, DEC);
}

void do_digitalWriteFast() {
  digitalWrite(PIN_MARKER, HIGH);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalWriteFast(PIN, HIGH);
    digitalWriteFast(PIN, LOW);
  }
  uint32_t delta_time = micros() - start_time;
  digitalWrite(PIN_MARKER, LOW);
  Serial.print("digitalWriteFast: ");
  Serial.println(delta_time, DEC);
}

void do_digitalWriteFastName() {
  digitalWrite(PIN_MARKER, HIGH);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalWriteFast(PIN_NAME, HIGH);
    digitalWriteFast(PIN_NAME, LOW);
  }
  uint32_t delta_time = micros() - start_time;
  digitalWrite(PIN_MARKER, LOW);
  Serial.print("digitalWriteFast(name): ");
  Serial.println(delta_time, DEC);
}


void do_digitalToggleFast() {
  digitalWrite(PIN_MARKER, HIGH);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalToggleFast(PIN);
    digitalToggleFast(PIN);
  }
  uint32_t delta_time = micros() - start_time;
  digitalWrite(PIN_MARKER, LOW);
  Serial.print("digitalToggleFast: ");
  Serial.println(delta_time, DEC);
}

void do_digitalToggleFastName() {
  digitalWrite(PIN_MARKER, HIGH);
  uint32_t start_time = micros();
  for (int i = 0; i < 1000; i++) {
    digitalToggleFast(PIN_NAME);
    digitalToggleFast(PIN_NAME);
  }
  uint32_t delta_time = micros() - start_time;
  digitalWrite(PIN_MARKER, LOW);
  Serial.print("digitalToggleFast(name): ");
  Serial.println(delta_time, DEC);
}


void loop() {
  if (Serial.available()) {
    while (Serial.available()) Serial.read();
    Serial.println("*** Paused ***");
    while (!Serial.available()) {}
    while (Serial.available()) Serial.read();
  }
  do_digitalWrite();
  Serial.flush();
  do_digitalWriteFast();
  Serial.flush();
  do_digitalWriteFastName();
  Serial.flush();
  do_digitalToggleFast();
  Serial.flush();
  do_digitalToggleFastName();
  Serial.flush();
  Serial.println();
  delay(1000);
}
