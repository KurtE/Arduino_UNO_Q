#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <vector>

#include "Arduino_RouterBridge.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include "memorySerial.h"


memorySerial MSerial;


uint32_t last_led_change_time_ms = 0;

#define STACK_SIZE 1024
#define PRIORITY 7

typedef struct {
  uint16_t    value;
  const char  *name;  
} value_to_name_t;

static const value_to_name_t axis_names[] = {
    {0, "lx"},
    {1, "ly"},
    {2, "rx"},
    {5, "ry"},
    {3, "L2"},
    {4, "R2"},
    {9, "R2/Rt"},
    {10, "L2/Lt"},
    {16, "dpad_x"},
    {17, "dpad_y"}
};

static const value_to_name_t button_names[] = {
    {305, "cross/B"},
    {306, "circle"},
    {307, "triangle/X"},
    {304, "square/A"},
    {308, "l1/Y"},
    {309, "r1/rs"},
    {310, "l2/ls"},
    {311, "r2_btn"},
    {312, "share"},
    {313, "options"},
    {316, "ps/XB"},
    {314, "l3/view"},
    {315, "r3/options"},
    {317, "touchpad/Stl Lt Bth"},
    {318, "Stick Rt Btn"},
    {158, "share"},
    {172, "guide"}
};


K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
struct k_thread thread1_data;
k_tid_t thread1_tid;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Bridge.begin();
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}
  delay(5000);
  Serial.println("\n*** Joystick test program starting ***");
  Serial.flush();

  Bridge.provide("joy_button_down", joy_button_down);
  Bridge.provide("joy_button_up", joy_button_up);
  Bridge.provide("joy_axis_motion", joy_axis_motion);
  //Bridge.provide("joy_hat_motion", joy_hat_motion);
  //Bridge.provide("joy_device_added", joy_device_added);
  //Bridge.provide("joy_device_removed", joy_device_removed);
  MSerial.begin();
}


void loop() {
  if ((millis() - last_led_change_time_ms) > 500) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    last_led_change_time_ms = millis();
  }
  uint8_t buffer[80];
  int cbRead;
  while ((cbRead = MSerial.read(buffer, sizeof(buffer))) > 0) {
    Serial.write(buffer, cbRead);
  }
  delay(25);
}


const char *map_button_to_name(int btn) {
  for(int i = 0; i < sizeof(button_names)/sizeof(button_names[0]); i++) {
    if (button_names[i].value == btn) return button_names[i].name;   
  }
  return nullptr;
}

void joy_button_down(int btn) {
  MSerial.print("BDN: ");
  MSerial.print(btn);
  const char *btn_name = map_button_to_name(btn);
  if (btn_name != nullptr) {
    MSerial.print(" (");
    MSerial.print(btn_name);
    MSerial.print(")");
  }
  MSerial.println();
}

void joy_button_up(int btn) {
  MSerial.print("BUP: ");
  MSerial.print(btn);
  const char *btn_name = map_button_to_name(btn);
  if (btn_name != nullptr) {
    MSerial.print(" (");
    MSerial.print(btn_name);
    MSerial.print(")");
  }
  MSerial.println();
}

const char *map_axis_to_name(int axis) {
  for(int i = 0; i < sizeof(axis_names)/sizeof(axis_names[0]); i++) {
    if (axis_names[i].value == axis) return axis_names[i].name;   
  }
  return nullptr;
}

void joy_axis_motion(std::vector<int> motions) {
  MSerial.print("Axis:");
  for (int i = 0; i < motions.size(); i += 2) {
    if ((i & 7) == 4) MSerial.println();
    MSerial.print(" ");
    MSerial.print(motions[i]);
    const char * axis_name = map_axis_to_name(motions[i]);
    if (axis_name != nullptr) {
      MSerial.print(" (");
      MSerial.print(axis_name);
      MSerial.print(")");
    }
    MSerial.print(":");
    MSerial.print(motions[i + 1]);
  }
  MSerial.println();
}
