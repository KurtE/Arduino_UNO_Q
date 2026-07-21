#include <digitalWriteFast_zephyr.h>
//#include "pinDefinitions.h"
#include "wiring_private.h"
using namespace zephyr::arduino;

#define PIN 2
#define PIN_MARKER 3
#define PIN_NAME PB_3

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nTest");
  while (!Serial && millis() < 5000)
    ;

#if 0
  for (int i = 0; i < NUM_OF_DIGITAL_PINS; i++) {
    Serial.print(i);
    Serial.print(":"); Serial.print((uint32_t)arduino_pins[i].port, HEX);
    const struct gpio_stm32_config *cfg = (gpio_stm32_config*)arduino_pins[i].port->config;
	  GPIO_TypeDef *gpio = (GPIO_TypeDef *)cfg->base;
    Serial.print(" "); Serial.print((uint32_t)gpio, HEX);
    Serial.print(" "); Serial.println(arduino_pins[i].pin);
  } 
#endif
  pinMode(PIN, OUTPUT);
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
  do_digitalWriteFast();
  do_digitalWriteFastName();
  do_digitalToggleFast();
  do_digitalToggleFastName();
  Serial.println();
  delay(1000);
}

