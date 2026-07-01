# SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
#
# SPDX-License-Identifier: MPL-2.0

from arduino.app_utils import *
import time
import pygame
import inputs

led_state = False

# This dict can be left as-is, since pygame will generate a
# pygame.JOYDEVICEADDED event for every joystick connected
# at the start of the program.
joysticks = {}

pygame.init()
pygame.joystick.init()
joystick_count = pygame.joystick.get_count()
print(joystick_count)
#joystick = pygame.joystick.Joystick(0)
#joystick.init()

def loop():
    global led_state
    event_processed = False
    #axis_motions: list[int] = [65536, 65536, 65536, 65536, 65536, 65536]
    axis_motion_list: list[int] = []

    for event in pygame.event.get() :
        event_processed = True
        if event.type == pygame.JOYAXISMOTION :
            axis_motion_list.append(event.axis)
            axis_motion_list.append(int(event.value * 1023))
            #axis_motions[event.axis] = event.value
            if event.axis == 0 :
                print("Left stick X: {}".format(event.value))
            elif event.axis == 1:
                print("Left stick y: {}".format(event.value))
            elif event.axis == 2:
                print("Right stick x: {}".format(event.value))
            elif event.axis == 3:
                print("Right stick y: {}".format(event.value))
            else :
            	print("Other axis {}: {}".format(event.axis,event.value))
        elif event.type == pygame.JOYHATMOTION :
            Bridge.notify("joy_hat_motion", event.value[0], event.value[1])
            print("Hat {}: {}".format(event.hat, event.value))
        elif event.type == pygame.JOYBUTTONDOWN :
            Bridge.notify("joy_button_down", event.button)
            print("button down: {}".format(event.button))
        elif event.type == pygame.JOYBUTTONUP :
            Bridge.notify("joy_button_up", event.button)
            print("button up: {}".format(event.button))
        # Handle hotplugging
        elif event.type == pygame.JOYDEVICEADDED:
            # This event will be generated when the program starts for every
            # joystick, filling up the list without needing to create them manually.
            Bridge.notify("joy_device_added", event.device_index)
            joy = pygame.joystick.Joystick(event.device_index)
            joysticks[joy.get_instance_id()] = joy
            print(f"Joystick {joy.get_instance_id()} connencted")
        
        elif event.type == pygame.JOYDEVICEREMOVED:
            Bridge.notify("joy_device_removed", event.instance_id)
            del joysticks[event.instance_id]
            print(f"Joystick {event.instance_id} disconnected")

        else :
            print("Other Event: {}".format(event))
    if event_processed :
        if len(axis_motion_list) :
            Bridge.notify("joy_axis_motion", axis_motion_list)
        print("---")
    #time.sleep(1)
    #led_state = not led_state
    #Bridge.call("set_led_state", led_state)

App.run(user_loop=loop)
