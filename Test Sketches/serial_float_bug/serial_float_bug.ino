#include <limits>
#ifdef ARDUINO_UNO_Q
#include <Arduino_RouterBridge.h>
#endif
void setup() {
#ifdef ARDUINO_UNO_Q
  Bridge.begin();
#endif
  Serial.begin(115200);

  delay(1000);
  Serial.print("MAX DBL: ");
  Serial.println(DBL_MAX);
  printk("MAX DBL: %lf float:%f\n", DBL_MAX, FLT_MAX);

  float f = 39.3;
  printk("\n\n*******************************************************\n");
  printk("inline %f > %f? %d\n", f, 4294967040.0, (f > 4294967040.0)? 1 : 0);
  print_through_function(f);
}


void loop() {
  float f = 39.3;
  double d = 55.9;
  int i = 3141;
  char print_buf[128];


  double double_val = 1100.0;
  double double_test = 4294967040.000000;

  printk("Test Double: %f %f %d\n", double_val, double_test, (double_val > double_test)? 1 : 0);
  double_test = 4294967000.0;
  while (double_val > double_test) double_test -= 1000.0;
  printk("    %f %f %d\n", double_val, double_test, (double_val > double_test)? 1 : 0);


  Serial.print("float f = ");
  Serial.println(f); /* ovf */
  if (snprintf(print_buf, sizeof(print_buf), "float f = %f using snprintf", f)) {
    Serial.println(print_buf);
  }

  Serial.print("double d = ");
  Serial.println(d); /* ovf */
  if (snprintf(print_buf, sizeof(print_buf), "double d = %f using snprintf", d)) {
    Serial.println(print_buf);
  }

  Serial.print("int i = ");
  Serial.println(i);

  delay(10000);
}

void  print_through_function(double val) {
  printk("though function %f > %f? %d\n", val, 4294967040.0, (val > 4294967040.0)? 1 : 0);
  printk("*******************************************************\n");
}
