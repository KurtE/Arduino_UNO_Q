void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //shiftIn(2, 3, MSBFIRST);
  //shiftOut(4, 5, LSBFIRST, 42);
  pinMode(LED_BUILTIN, OUTPUT);

}

uint8_t loop_count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  loop_count++;
  digitalWrite(LED_BUILTIN, loop_count & 1);
  shiftOut(4, 5, LSBFIRST, loop_count);
  delay(1);
}
