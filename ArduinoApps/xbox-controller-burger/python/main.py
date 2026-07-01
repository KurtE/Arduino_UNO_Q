# SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
#
# SPDX-License-Identifier: MPL-2.0

from arduino.app_utils import *
import time
import pygame
import inputs

led_state = False

pygame.init()
pygame.joystick.init()
joystick_count = pygame.joystick.get_count()
print(joystick_count)
joystick = pygame.joystick.Joystick(0)
joystick.init()

def loop():
    global led_state

    for event in pygame.event.get() :
        if event.type == pygame.JOYAXISMOTION :
            if event.axis == 0 :
                print("Left stick X: {}".format(event.value))
            elif event.axis == 1:
                print("Left stick y: {}".format(event.value))
        elif event.type == pygame.JOYBUTTONDOWN :
            print("button down: {}".format(event.button))
    
    #time.sleep(1)
    #led_state = not led_state
    #Bridge.call("set_led_state", led_state)

App.run(user_loop=loop)
