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
  for (int r = 0; r < repeat; r++) {
    for (int i = 0; i < frameCount; i++) {
      uint32_t start_time = micros();
      matrixWrite((uint32_t*)frames[i]);
      delay(frameDelay);
    }
  }
}

void display_string(char *buffer) {
  Monitor.write(buffer);
  matrix.beginDraw();
  int cch = strlen(buffer);
  if (cch <= 2) {
    matrix.textFont(Font_5x7);
    int x_start = (13 - cch*5) / 2;
    matrix.text(buffer, (x_start < 0)? 0 : x_start, 1);  
  } else {
    matrix.textFont(Font_4x6);
    if ((cch == 4) && (buffer[0] == '0') && (buffer[1] == '.')) {
      matrix.text(&buffer[1], 0, 1);  
    } else {
      matrix.text(buffer, 0, 1);  
    }
  }
  
    matrix.endDraw();
}


String city = "Anacortes";

void loop() {
  String weather_forecast;
  int cur_temp;
  int cur_precip;  // 100ths of an inch
  bool ok =  Bridge.call("get_weather_forecast", city).result(weather_forecast);
  if (ok) {
    Bridge.call("get_weather_temp").result(cur_temp);
    Monitor.print(cur_temp);
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

    // lets show temp
    char buffer[10];
    sprintf(buffer,"%d", cur_temp);
    display_string(buffer);
    delay(2000);

    // maybe show precip
    Bridge.call("get_weather_precip").result(cur_precip);
    Monitor.print(cur_precip);
    if (cur_precip > 0) {
      if (cur_precip > 100) {
        sprintf(buffer, "%d.%d", cur_precip / 100, (cur_precip / 10) % 10);
      } else if (cur_precip > 10) {
        sprintf(buffer, ".%d", cur_precip % 100);
      } else {
        sprintf(buffer, ".0%d", cur_precip % 100);
      }
      display_string(buffer);
      delay(2000);
    }
    
  }
}
