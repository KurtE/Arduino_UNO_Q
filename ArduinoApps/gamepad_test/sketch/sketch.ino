// SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
//
// SPDX-License-Identifier: MPL-2.0

#include "Arduino_RouterBridge.h"
#include <vector>

uint32_t last_led_update_time = 0;
#define BLINK_TIME 500

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
}

void loop() {
    if ((millis() - last_led_update_time) >= BLINK_TIME) {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        last_led_update_time = millis();
    }
}

void joy_button_down(int btn) {
    printk("BDN: %d\n", btn);
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
