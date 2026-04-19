void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

uint8_t loop_count = 0;
void loop() {
  // put your main code here, to run repeatedly:
  loop_count++;
  digitalWrite(2, loop_count & 1);
  digitalWrite(3, loop_count & 2);
  delay(100);

}
