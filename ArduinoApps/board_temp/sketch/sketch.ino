#include <Arduino_RouterBridge.h>
#include "zephyr_die_temp.h"

String mpu_thermal = "thermal_zone3";
String mpu_type;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}
  delay(2000);
  Bridge.call("get_mpu_type", mpu_thermal).result(mpu_type);
  
}


void loop() {
  float mpu_temp;
  double mcu_temp;

  if (Serial.available()) {
    String str = Serial.readString();
    str.trim();
    if (str.length() == 1) {
      mpu_thermal = "thermal_zone";
      mpu_thermal.concat(str);
    } else {
      mpu_thermal = str;
    }
  }
  Bridge.call("get_mpu_type", mpu_thermal).result(mpu_type);
  Bridge.call("get_mpu_temp", mpu_thermal).result(mpu_temp);

  Serial.print(" Qualcomm(");
  Serial.print(mpu_type);
  Serial.print(") C:");
  Serial.print(mpu_temp, 2);
  if (!isnan(mpu_temp)) {
    Serial.print(" F:");
    Serial.print(mpu_temp * 1.8 + 32.0, 2);
  }

  mcu_temp = CPUTemperature();
  Serial.print(" STM32 C:");
  Serial.print(mcu_temp, 2);
  if (!isnan(mcu_temp)) {
    Serial.print(" F:");
    Serial.print(mcu_temp * 1.8 + 32.0, 2);
  }

  Serial.println();

  delay(5000);
  
}
