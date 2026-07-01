// SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
//
// SPDX-License-Identifier: MPL-2.0

#include "Arduino_RouterBridge.h"
#include <vector>
#include <Dynamixel2Arduino.h>

uint32_t last_led_update_time = 0;
#define BLINK_TIME 500
#define LEFT_WHEEL_ID 1
#define RIGHT_WHEEL_ID 2

enum { JOY_LEFT_X = 0,
       JOY_LEFT_Y,
       JOY_RIGHT_X,
       JOY_RIGHT_Y,
       JOY_LEFT_TRIGGER,
       JOY_RIGHT_TRIGGER };

const int DXL_DIR_PIN = 2;  // DYNAMIXEL Shield DIR PIN
const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
#define DEBUG_SERIAL Monitor
#define DXL_SERIAL Serial1
uint8_t servos_found = 0xff;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Bridge.begin();
  DEBUG_SERIAL.begin();
  Bridge.provide("joy_button_down", joy_button_down);
  Bridge.provide("joy_button_up", joy_button_up);
  Bridge.provide("joy_axis_motion", joy_axis_motion);
  Bridge.provide("joy_hat_motion", joy_hat_motion);
  Bridge.provide("joy_device_added", joy_device_added);
  Bridge.provide("joy_device_removed", joy_device_removed);


  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  DEBUG_SERIAL.print("After dxl.begin\n");
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  DEBUG_SERIAL.println("Set protocol");


  // Set both servos into Velocity mode
  // Turn off torque when configuring items in EEPROM area

}


char buffer[128];
DYNAMIXEL::InfoFromPing_t ping_info[32];

void FindServos(void) {
  servos_found = 0;
  DEBUG_SERIAL.println("  Try Protocol 2 - broadcast ping: ");
  DEBUG_SERIAL.flush();  // flush it as ping may take awhile...
  if (uint8_t count_pinged = dxl.ping(DXL_BROADCAST_ID, ping_info,
                                      sizeof(ping_info) / sizeof(ping_info[0]))) {
    //DEBUG_SERIAL.print("Detected Dynamixel : \n");
    DEBUG_SERIAL.println("Detected Dynamixel :");
    for (int i = 0; i < count_pinged; i++) {
      sprintf(buffer, "    %u, Model:%d, Ver:%d\n", ping_info[i].id, ping_info[i].model_number, ping_info[i].firmware_version);
      DEBUG_SERIAL.print(buffer);
      if (ping_info[i].id == LEFT_WHEEL_ID) {
        DEBUG_SERIAL.println("Found Left wheel");
        dxl.torqueOff(LEFT_WHEEL_ID);
        dxl.setOperatingMode(LEFT_WHEEL_ID, OP_VELOCITY);
        dxl.torqueOn(LEFT_WHEEL_ID);
        servos_found |= 1;
      } else if (ping_info[i].id == RIGHT_WHEEL_ID) {
        dxl.torqueOff(RIGHT_WHEEL_ID);
        dxl.setOperatingMode(RIGHT_WHEEL_ID, OP_VELOCITY);
        dxl.torqueOn(RIGHT_WHEEL_ID);
        DEBUG_SERIAL.println("Found right wheel");
        servos_found |= 2;
      }
    }
  } else {
    DEBUG_SERIAL.print("Broadcast returned no items : ");
    DEBUG_SERIAL.println(dxl.getLastLibErrCode());
    //printk("Broadcast returned no items: %d\n", dxl.getLastLibErrCode());
  }
}

int16_t cur_wheel_velocity = 0;
volatile int16_t new_wheel_velocity = 0;

void loop() {
  Bridge.update();
  if ((millis() - last_led_update_time) >= BLINK_TIME) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    last_led_update_time = millis();
  }

  if (servos_found == 0xff) {
    FindServos();
  }


  if ((new_wheel_velocity != cur_wheel_velocity) && (servos_found == 3)) {
      DEBUG_SERIAL.println("change Velocity");
      cur_wheel_velocity = new_wheel_velocity;
      dxl.setGoalVelocity(LEFT_WHEEL_ID, cur_wheel_velocity);
      dxl.setGoalVelocity(RIGHT_WHEEL_ID, cur_wheel_velocity);
  }
}

void joy_button_down(int btn) {
  sprintf(buffer, "BDN: %d\n", btn);
  DEBUG_SERIAL.print(buffer);
  if (btn == 3) {
    servos_found = 0xff;
  }
}

void joy_button_up(int btn) {
  sprintf(buffer, "BUP: %d\n", btn);
  DEBUG_SERIAL.print(buffer);
}

void joy_axis_motion(std::vector<int> motions) {
  for (int i = 0; i < motions.size(); i += 2) {
    sprintf(buffer, "%d:%d ", motions[i], motions[i + 1]);
    DEBUG_SERIAL.print(buffer);
    if (motions[i] == JOY_LEFT_Y) {
      new_wheel_velocity = motions[i+1];
    }
  }
  DEBUG_SERIAL.print("\n");
}

void joy_hat_motion(int x, int y) {
  sprintf(buffer, "Hat: %d %d\n", x, y);
  DEBUG_SERIAL.print(buffer);
}

void joy_device_added(int index) {
  sprintf(buffer, "\nJoystick device added: %d\n", index);
  DEBUG_SERIAL.print(buffer);
}
void joy_device_removed(int index) {
  sprintf(buffer, "\nJoystick device removed: %d\n", index);
  DEBUG_SERIAL.print(buffer);
}
