// SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
//
// SPDX-License-Identifier: MPL-2.0

#include "Arduino_RouterBridge.h"
#include <vector>
#include <Dynamixel2Arduino.h>

uint32_t last_led_update_time = 0;
#define BLINK_TIME 500


#define DXL_SERIAL   Serial
#define DEBUG_SERIAL Monitor
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Bridge.begin();
    Monitor.begin();
    Bridge.provide("joy_button_down", joy_button_down);
    Bridge.provide("joy_button_up", joy_button_up);
    Bridge.provide("joy_axis_motion", joy_axis_motion);
    Bridge.provide("joy_hat_motion", joy_hat_motion);
    Bridge.provide("joy_device_added", joy_device_added);
    Bridge.provide("joy_device_removed", joy_device_removed);


    // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
    dxl.begin(1000000);
    printk("After dxl.begin\n");
    // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    printk("Set protocol");

     FindServos();


}


char buffer[128];
DYNAMIXEL::InfoFromPing_t ping_info[32];

void FindServos(void) {
  DEBUG_SERIAL.println("  Try Protocol 2 - broadcast ping: ");
  DEBUG_SERIAL.flush(); // flush it as ping may take awhile... 
      
  if (uint8_t count_pinged = dxl.ping(DXL_BROADCAST_ID, ping_info, 
    sizeof(ping_info)/sizeof(ping_info[0]))) {
    //DEBUG_SERIAL.print("Detected Dynamixel : \n");
    printk("Detected Dynamixel :");
    for (int i = 0; i < count_pinged; i++)
    {
      //sprintf(buffer, "    %u, Model:%d, Ver:%d\n", ping_info[i].id, ping_info[i].model_number, ping_info[i].firmware_version);
      //DEBUG_SERIAL.print(buffer);
      printk("    %u, Model:%d, Ver:%d\n", ping_info[i].id, ping_info[i].model_number, ping_info[i].firmware_version);
#if 0      
      DEBUG_SERIAL.print("    ");
      DEBUG_SERIAL.print(ping_info[i].id, DEC);
      DEBUG_SERIAL.print(", Model:");
      DEBUG_SERIAL.print(ping_info[i].model_number);
      DEBUG_SERIAL.print(", Ver:");
      DEBUG_SERIAL.println(ping_info[i].firmware_version, DEC);
#endif      
      //g_servo_protocol[i] = 2;
    }
  }else{
    //DEBUG_SERIAL.print("Broadcast returned no items : ");
    //DEBUG_SERIAL.println(dxl.getLastLibErrCode());
    printk("Broadcast returned no items: %d\n", dxl.getLastLibErrCode());
  }
}

void loop() {
    if ((millis() - last_led_update_time) >= BLINK_TIME) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        last_led_update_time = millis();
    }
}

void joy_button_down(int btn) {
    printk("BDN: %d\n", btn);
    if (btn == 3) {
        FindServos();
    }
}

void joy_button_up(int btn) {
    printk("BUP: %d\n", btn);
}

void joy_axis_motion(std::vector<int> motions) {
    for (int i = 0; i < motions.size(); i += 2) {
        printk("%d:%d ", motions[i], motions[i+1]);
    }
    printk("\n");
}

 void joy_hat_motion(int x, int y) {
    printk("Hat: %d %d\n", x, y);
 }

void joy_device_added (int index) {
    printk("\nJoystick device added: %d\n", index);
}
void joy_device_removed (int index) {
    printk("\nJoystick device removed: %d\n", index);
}
