// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_RouterBridge.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <vector>

#include <Modulino.h>
#include <Arduino_Modulino.h>
#include "weather_frames.h"

String city;
#if defined __has_include
#if __has_include ("location.h")
#include "location.h"
#else
#define DEFAULT_CITY "Los Angeles"
#endif
#else
#warning "__has_include not defined"
#endif

// Create a ModulinoButtons object
ModulinoButtons buttons;
#define THREAD_STACK_SIZE    500
#define THREAD_PRIORITY      7

K_THREAD_STACK_DEFINE(btn_stack_area, THREAD_STACK_SIZE);
K_MUTEX_DEFINE(btn_mutex);

k_tid_t btn_tid;
struct k_thread btn_thread_data{};


volatile bool button_a = true;
volatile bool button_b = true;
volatile bool button_c = true;
bool buttons_found = false;
volatile uint8_t buttons_changed = 0;
// Also try a Qwiic button
#include <SparkFun_Qwiic_Button.h>
QwiicButton qbutton;
bool qbutton_found;

Arduino_LED_Matrix matrix;

#define MIN_SPEED_TO_DISPLAY 2.0

static const char* wind_directions[] = { "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
                                         "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW" };

enum { TEMP = 0,
       PRECIP,
       SPEED,
       DIR };

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // just in case we print here
  matrix.begin();
  matrix.textFont(Font_5x7);
  matrix.textSize(1, 1);
  matrix.stroke(127, 127, 127);
  matrix.clear();

  // Initialize Modulino I2C communication
  Wire1.begin();
  Modulino.begin(Wire1);
  // Detect and connect to buttons module
  //printk("After Modulino begin\n");
  buttons_found = buttons.begin();

  k_mutex_init(&btn_mutex);
  //printk("After buttons begin %u\n", buttons_found);
  if (buttons_found) {
    buttons.setLeds(true, true, true);

    // lets create a thread to monitor the buttons
    btn_tid = k_thread_create(&btn_thread_data, btn_stack_area,
                            K_THREAD_STACK_SIZEOF(btn_stack_area),
                            btnScanEntryPoint,
                            NULL, NULL, NULL,
                            THREAD_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(btn_tid, "btn_scan");

    
    
  }

  qbutton_found = qbutton.begin(SFE_QWIIC_BUTTON_DEFAULT_ADDRESS, Wire1);
  //printk("QButton begin: %u\n", qbutton_found);
  if (qbutton_found && button_a) qbutton.LEDon(64);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  Bridge.begin();
  Monitor.begin();

  bool ok = Bridge.call("get_weather_city", city).result(city);
  if (!ok || (city == "")) city = DEFAULT_CITY;
  
  digitalWrite(LED_BUILTIN, LOW);
}


/* make global so I can decide not to call through bridge every call */

String weather_forecast = "";
std::vector<float> cur_weather_data;

uint32_t last_forcast_time = (uint32_t)-1;
#define TIME_BETWEEN_CALLS_MS (1 * 60000) /* 1 minutes */

void loop() {
  // Modulino buttons updated in thread
  //printk("Loop");
  k_mutex_lock(&btn_mutex, K_FOREVER);
  //printk(" lock");
  digitalWrite(LED_BUILTIN, HIGH);

  if (qbutton_found) {
    if (qbutton.hasBeenClicked()) {
        button_a = !button_a;
        //printk("QButton pressed: %u\n", button_a);
        if (button_a) qbutton.LEDon(64);
        else qbutton.LEDoff();
        qbutton.clearEventBits();
        buttons_changed = 1;
    } else if (buttons_changed & 1) {
        // can be cleaner 
        if (button_a) qbutton.LEDon(64);
        else qbutton.LEDoff();
    }
  }
  
  // Update the LEDs above buttons, depending on the variables value
  if (buttons_changed) {
    buttons.setLeds(button_a, button_b, button_c);
    buttons_changed = 0;
  } 

  k_mutex_unlock(&btn_mutex);
  //printk(" unlock");
  
  char buffer[80];
  if ((last_forcast_time == (uint32_t)-1) || ((millis() - last_forcast_time) > TIME_BETWEEN_CALLS_MS)) {
    //printk(" Bridge");
    bool ok = Bridge.call("get_weather_forecast", city).result(weather_forecast);
    if (!ok) {
      printk("Failed to get weather forecast\n");
      delay(5000);
      Monitor.println("Failed to get_weather_forecast");
      return;
    }
    if (!Bridge.call("get_weather_data").result(cur_weather_data)) {
      printk("Failed to get weather data\n");
      return;
    }
    sprintf(buffer, "%.1f %.1f %.1f %f,1 ", cur_weather_data[TEMP], cur_weather_data[PRECIP], cur_weather_data[SPEED], cur_weather_data[DIR]);
    Monitor.print(buffer);
    last_forcast_time = millis();
  }
  //printk("\n");
  if (weather_forecast == "sunny") {
    matrix.loadSequence(sunny);
    playRepeat(5);
  } else if (weather_forecast == "cloudy") {
    matrix.loadSequence(cloudy);
    playRepeat(5);
  } else if (weather_forecast == "rainy") {
    matrix.loadSequence(rainy);
    playRepeat(10);
  } else if (weather_forecast == "snowy") {
    matrix.loadSequence(snowy);
    playRepeat(5);
  } else if (weather_forecast == "foggy") {
    matrix.loadSequence(foggy);
    playRepeat(3);
  }

  // lets show temp
  if (!button_a) {
    sprintf(buffer, "%d", (int)(cur_weather_data[TEMP] + 0.5));
    display_string(buffer);
    delay(2000);

    // maybe show precip
    //Monitor.print(cur_precip);
    if (cur_weather_data[PRECIP] > 0) {
      if (cur_weather_data[PRECIP] >= 1.0) {
        sprintf(buffer, "%d", (int)cur_weather_data[PRECIP]);
      } else if (cur_weather_data[PRECIP] >= 0.10) {
        sprintf(buffer, "%.1f", cur_weather_data[PRECIP]);
      } else {
        sprintf(buffer, "%f.2", cur_weather_data[PRECIP]);
      }
      display_string(buffer);
      delay(2000);
    }
    //Monitor.println();

    if (cur_weather_data[SPEED] >= MIN_SPEED_TO_DISPLAY) {
      // first display wind direction
      int dir_index = int((cur_weather_data[DIR] + 12.25) / 22.5);
      display_string(wind_directions[(dir_index < 16) ? dir_index : 0]);
      delay(2000);
      sprintf(buffer, "%d", (int)cur_weather_data[SPEED]);
      display_string(buffer);
      delay(2000);
    }
  } else {
    // Lets try a quick and dirty scrolling of text
    matrix.textFont(Font_5x7);
    matrix.textScrollSpeed(100);
    matrix.clear();
    matrix.beginText(0, 0, 127, 0, 0);  // X, Y, then R, G, B
    matrix.print(" ");
    if (button_b) {
      matrix.print(city);
    }
    sprintf(buffer, " T %d", (int)(cur_weather_data[TEMP] + 0.5));
    matrix.print(buffer);
    if (cur_weather_data[PRECIP] > 0) {
      sprintf(buffer, " R %.1f");
      matrix.print(buffer);
    }
    if (cur_weather_data[SPEED] >= MIN_SPEED_TO_DISPLAY) {
      // first display wind direction
      int dir_index = int((cur_weather_data[DIR] + 12.25) / 22.5);
      matrix.print(" W ");
      matrix.print(wind_directions[(dir_index < 16) ? dir_index : 0]);
      sprintf(buffer, " %d", (int)cur_weather_data[SPEED]);
      matrix.print(buffer);
    }
    // now try to scroll this text
    matrix.endText(SCROLL_LEFT);
  }
}

void playRepeat(int repeat_count) {
  for (int i = 0; i < repeat_count; i++) {
    matrix.playSequence();
  }
}

void display_string(const char* buffer) {
  Monitor.write(buffer);
  matrix.beginDraw();
  matrix.clear();
  int cch = strlen(buffer);
  if (cch <= 2) {
    matrix.textFont(Font_5x7);
    int x_start = (13 - cch * 5) / 2;
    matrix.text(buffer, (x_start < 0) ? 0 : x_start, 1);
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


void btnScanEntryPoint(void *p1, void *p2, void *p3) {
  UNUSED(p1);
  UNUSED(p2);
  UNUSED(p3);
  while (true) {
    k_mutex_lock(&btn_mutex, K_FOREVER);
    if (buttons.update()) {
    // You can use either index (0=A, 1=B, 2=C) or letter ('A', 'B', 'C') to check buttons
    // Below we use the letter-based method for better readability
      if (buttons.isPressed('A')) {
        //Serial.println("Button A pressed!");
        button_a = !button_a;
        buttons_changed |= 1;
      } else if (buttons.isPressed("B")) {
        //Serial.println("Button B pressed!");
        button_b = !button_b;
        buttons_changed |= 2;
      } else if (buttons.isPressed('C')) {
        //Serial.println("Button C pressed!");
        button_c = !button_c;
        buttons_changed |= 4;
      }

    }
    k_mutex_unlock(&btn_mutex);
    k_msleep(250);
  }
}
