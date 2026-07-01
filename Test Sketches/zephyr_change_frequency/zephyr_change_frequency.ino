//#ifndef NUM_OF_DIGITAL_PINS
//#define NUM_OF_DIGITAL_PINS LED_BUILTIN
//#endif

#define PWM_PIN 2
#define STEP_SIZE 128
uint16_t pwm_pin = PWM_PIN;

uint32_t cur_frequency = 1000000;
void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  pinMode(3, OUTPUT);
  // Start of the PWM...
  analogWriteResolution(10);
  analogWrite(pwm_pin, 512);
  delay(100);

  analogWriteFrequency(pwm_pin, cur_frequency);

  // Lets see if we can print out all of the pins for frequency.
  Serial.println("\n\n*** Change Frequency test ***");
  Serial.print("NUM_OF_DIGITAL_PINS: ");
  Serial.println(NUM_OF_DIGITAL_PINS);
  for (int pin = 0; pin < NUM_OF_DIGITAL_PINS; pin++) {
    float freq = analogWriteFrequency(pin);
    if (freq > 0.0f) {
      Serial.print(pin);
      Serial.print(" = ");
      Serial.println(freq, 2);
    }
  }
}

uint16_t duty = 0;
void loop() {
  if (Serial.available()) {
    uint32_t new_freq = 0;
    int ch;
    ch = Serial.read();
    if (ch == '#') {
      uint8_t pin = 0;
      while ((ch = Serial.read()) >= 0) {
        if ((ch >= '0') && (ch <= '9')) {
          pin = pin * 10 + ch - '0';
        }
      }
      Serial.print("New PWM PIN: ");
      Serial.println(pin);
      pwm_pin = pin;
      return;
    }
    if ((ch >= '0') && (ch <= '9')) {
      new_freq = new_freq * 10 + ch - '0';
    }
    while ((ch = Serial.read()) >= 0) {
      if ((ch >= '0') && (ch <= '9')) {
        new_freq = new_freq * 10 + ch - '0';
      }
    }
    Serial.print("Set new Freq: ");
    Serial.print(new_freq);
    analogWriteFrequency(pwm_pin, new_freq);
    analogWrite(pwm_pin, duty);
    int err = analogWriteLastStatus();
    if (err != 0) {
      Serial.print(" -- failed(");
      Serial.print(err);
      Serial.println(")");
      analogWriteFrequency(pwm_pin, cur_frequency);
    } else {
      cur_frequency = new_freq;
      Serial.println();
    }
  }
  delay(10);
  digitalWrite(3, !digitalRead(3));
  analogWrite(pwm_pin, duty);
  int err = analogWriteLastStatus();
  if (err != 0) {
    Serial.print("Last Status = ");
    Serial.println(err);
  }
  duty += STEP_SIZE;
  if (duty == 1024) duty = 1023;
  else if (duty > 1024) duty = 0;
}
