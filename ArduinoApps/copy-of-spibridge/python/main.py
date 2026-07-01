import time
import os
import numpy as np
import spibridge
import schedule
from arduino.app_utils import App

import joystickbridge as joystick
import struct
latest_packet = None


AXIS_ORDER = [
    0,   # lx
    1,   # ly
    2,   # rx
    5,   # ry
    3,   # l2 analog
    4,   # r2 analog
    16,  # dpad_x
    17,  # dpad_y
]

BUTTON_ORDER = [
    305,  # cross
    306,  # circle
    307,  # triangle
    304,  # square

    308,  # L1
    309,  # R1
    310,  # L2 click
    311,  # R2 click

    312,  # share
    313,  # options
    316,  # ps

    314,  # L3
    315,  # R3

    317,  # touchpad
]
# -------------------------
# SPI CONFIG
# -------------------------
print(spibridge.config_speed(2000000))
print(spibridge.config_mode(0))
print(spibridge.config_bits(8))
print(spibridge.config_bytes_to_read(2048))
print(spibridge.configWriteCommand("bytes", [0x1A, 0, 0, 0, 0]))


def pack_joystick(axis_values, button_values, num_axes, num_buttons):
    # axis_values is already a list of length num_axes
    axes = [int(v) for v in axis_values[:num_axes]]

    # Build button bitmask from list of 0/1
    buttons = 0
    for idx, val in enumerate(button_values[:num_buttons]):
        if val:
            buttons |= (1 << idx)

    # Pack into binary struct
    fmt = "<" + ("h" * num_axes) + "H"
    return struct.pack(fmt, *axes, buttons)


def get_cpu_temp():
    """
    Obtains the current value of the CPU temperature.
    :returns: Current value of the CPU temperature if successful, zero value otherwise.
    :rtype: float
    """
    # Initialize the result.
    result = 0.0
    # The first line in this file holds the CPU temperature as an integer times 1000.
    # Read the first line and remove the newline character at the end of the string.
    if os.path.isfile('/sys/class/thermal/thermal_zone0/temp'):
        with open('/sys/class/thermal/thermal_zone0/temp') as f:
            line = f.readline().strip()
        # Test if the string is an integer as expected.
        if line.isdigit():
            # Convert the string with the CPU temperature to a float in degrees Celsius.
            result = float(line) / 1000
    
    # Give the result back to the caller.

    return result

def read_temp():
    """This function is called repeatedly by the App framework."""
    # You can replace this with any code you want your App to run repeatedly.
    #print('Current CPU temperature is {:.2f} degrees Celsius.'.format(get_cpu_temp()))
    print('\t {:.2f} degrees Farhrenheit.'.format(get_cpu_temp()*1.8 + 32))



def scheduled_send_joystick():
    global latest_packet
    if latest_packet is not None:
        spibridge.writeBytes(list(latest_packet))


    
def main():

    schedule.every(1).seconds.do(read_temp)
    schedule.every(0.01).seconds.do(scheduled_send_joystick)

    print("=== PS4 EVDEV Joystick Test ===")

    # Metadata
    try:
        name = joystick.getName()
        print("Joystick Name:", name)

        num_axes = int(joystick.getNumAxes()["num_axes"])
        print("Number of Axes:", num_axes)
        
        num_buttons = int(joystick.getNumButtons()["num_buttons"])
        print("Number of Buttons:", num_buttons)

        axis_map = joystick.getAxisMap()
        print("RAW axis_map:", axis_map)

        button_map = joystick.getButtonMap()
        print("RAW button_map:", button_map)

    except Exception as e:
        print("Error reading joystick metadata:", e)
        return

    print("\n=== Live Joystick State ===")
   
    while True:
        schedule.run_pending()
        
        data = spibridge.readBytes(n=5)
        
        try:
            state = joystick.getState()
    
            # Convert keys to integers
            axes = {int(k): v for k, v in state.get("axes", {}).items()}
            buttons = {int(k): v for k, v in state.get("buttons", {}).items()}
    
            # Build fixed arrays
            axis_values = [axes.get(code, 0) for code in AXIS_ORDER]
            button_values = [buttons.get(code, 0) for code in BUTTON_ORDER]

            global latest_packet
            latest_packet = pack_joystick(axis_values, button_values, num_axes, num_buttons)

    
        except Exception as e:
            print("Error reading joystick state:", e)

    time.sleep(0.005)

if __name__ == "__main__":
    main()
