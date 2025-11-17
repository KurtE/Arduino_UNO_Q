// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

#include "weather_frames.h"

// TODO: those will go into an header file.
Arduino_LED_Matrix matrix;
//extern "C" void matrixWrite(const uint32_t* buf);
//extern "C" void matrixBegin();

void setup() {

  matrix.begin();
  matrix.textFont(Font_5x7);
  matrix.textSize(1,1);
  matrix.stroke(127,127,127);
  matrix.clear();
  Bridge.begin();
  Monitor.begin();
  
}

void playAnimation(const uint32_t* frames[], int frameCount, int repeat, int frameDelay) {
  uint32_t total_delta_time = 0;
  for (int r = 0; r < repeat; r++) {
    for (int i = 0; i < frameCount; i++) {
      uint32_t start_time = micros();
      matrixWrite((uint32_t*)frames[i]);
      total_delta_time += (micros() - start_time);
      delay(frameDelay);
    }
  }
  Monitor.print(total_delta_time);
  Monitor.print(frameCount * repeat);
  Monitor.println(total_delta_time / (frameCount * repeat));
}

String city = "Anacortes";

void loop() {
  String weather_forecast;
  int cur_temp;
  bool ok =  Bridge.call("get_weather_forecast", city).result(weather_forecast);
  if (ok) {
    Bridge.call("get_weather_temp").result(cur_temp);
    Monitor.print(cur_temp);
 #if 1
    if (weather_forecast == "sunny") {
      playAnimation(SunnyFrames, 2, 20, 500);
    } else if (weather_forecast == "cloudy") {
      playAnimation(CloudyFrames, 4, 20, 500);
    } else if (weather_forecast == "rainy") {
      playAnimation(RainyFrames, 3, 16, 200);
    } else if (weather_forecast == "snowy") {
      playAnimation(SnowyFrames, 3, 5, 650);
    } else if (weather_forecast == "foggy") {
      playAnimation(FoggyFrames, 2, 5, 660);
    }
#endif
    // lets show temp
    char buffer[10];
    sprintf(buffer,"%d", cur_temp);
    Monitor.write(buffer);
    matrix.beginDraw();
    int x_start = (13 - strlen(buffer)*5) / 2;
    matrix.text(buffer, (x_start < 0)? 0 : x_start, 1);
    matrix.endDraw();
    delay(2000);
  }
}
