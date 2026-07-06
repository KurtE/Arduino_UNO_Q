from arduino.app_utils import *
import time
import joystickbridge as joystick


print("=== Burger Simple Joystick Test ===")
print(">> Waiting for Joystick")

joystick_connected = False
joystick_name = None
num_axes = 0
num_button = 0
axis_map = None
button_map = None
debug_output = True

BUTTON_NAMES = {
    305: "cross",      # BTN_SOUTH
    306: "circle",     # BTN_EAST
    307: "triangle",   # BTN_NORTH
    304: "square",     # BTN_WEST

    308: "l1/Y",         # BTN_TL
    309: "r1",         # BTN_TR
    310: "l2_btn",     # BTN_TL2
    311: "r2_btn",     # BTN_TR2

    312: "share",      # BTN_SELECT
    313: "options",    # BTN_START
    316: "ps",         # BTN_MODE

    314: "l3",         # BTN_THUMBL
    315: "r3/options",         # BTN_THUMBR

    317: "touchpad/Stl Lt Bth",   # Touchpad click

    318: "Stick Rt Btn",

    # Firmware-dependent:
    158: "share",   # sometimes "back" on older firmware
    172: "guide",   # sometimes "xbox" on older firmware
}

# ----------------------------------------------------
AXIS_NAMES = {
    0: "lx",      # ABS_X
    1: "ly",      # ABS_Y
    2: "rx",      # ABS_RX
    5: "ry",      # ABS_RY

    3: "L2 Analog", 
    4: "R2 Analog", 
    9: "R2 Analog", 
    10: "L2 Analog", 

    16: "dpad_x", # ABS_HAT0X
    17: "dpad_y", # ABS_HAT0Y
}

def low_bit_order(n):
    """Return the index (0‑based) of the lowest set bit in n."""
    if n == 0:
        return 0  # or raise ValueError if you prefer
    return (n & -n).bit_length() - 1


def loop():
    global joystick_connected, name, num_axes, num_button, axis_map, button_map

    if not joystick_connected:
            
        try:
            connected = joystick.getConnected()
            is_connected = connected['connected']
            print ("Connected: ", is_connected, type(is_connected))
            if is_connected == False:
                print(".")
                time.sleep(2.5)
                return
                
        except Exception as e:
            print("Error waing for joystick:", e)
            return
            
        print(">> Joystick connected")
        joystick_connected = True
    
    
        try:
            joystick_name = joystick.getName()
    
            num_axes = joystick.getNumAxes()
            num_axes = num_axes["num_axes"]
    
            num_buttons = joystick.getNumButtons()
            num_buttons = num_buttons["num_buttons"]
    
            axis_map = joystick.getAxisMap()
            axis_map = axis_map["axis_map"]
    
            button_map = joystick.getButtonMap()
            button_map = button_map["button_map"]

            if debug_output:
                print("Joystick Name:", joystick_name)
                print("Number of Axes:", num_axes)
                print("Number of Buttons:", num_buttons)
                print("RAW axis_map:", axis_map)
                print("RAW button_map:", button_map)
                
        
        except Exception as e:
            print("Error reading joystick metadata:", e)
            return
    
    # Live loop
    try:
        state = joystick.getState()
        axes_changed = state["axes_changed"]
        buttons_changed = state["buttons_changed"]

        if axes_changed or buttons_changed:
            if debug_output:
                print("--------------")

            if buttons_changed:
                #buttons = {int(k): v for k, v in state.get("buttons", {}).items()}
                #buttons = {int(k): v for k, v in state.get("buttons", []).items()}
                buttons = state["buttons"]
                if debug_output:
                    print("Buttons Changed:", hex(buttons_changed), end="")

                button_index = 0
                while buttons_changed:
                    lowest_bit_set = low_bit_order(buttons_changed)
                    button_index = button_index + lowest_bit_set
                    if debug_output:
                        print(" BTN: ", button_index, end="")
                    if (button_map):
                        btn = button_map[button_index]
                        if debug_output:
                            print("(", btn,":", BUTTON_NAMES[btn], ")", end="")
                        
                    if (buttons[button_index] != 0):
                        if debug_output:
                            print(" Pressed", end="")
                        Bridge.notify("joy_button_down", btn)

                    else:
                        Bridge.notify("joy_button_up", btn)
                        if debug_output:
                            print(" Released", end="")

                    buttons_changed = buttons_changed >> (lowest_bit_set + 1)
                    button_index = button_index + 1 # need to one bias the index
                    
                if debug_output:
                    print("")
                
            if axes_changed:
                axes = state["axes"]
                if debug_output:
                    print("Axes Changed:", hex(axes_changed), end="")
                axis_motion_list: list[int] = []

                axis_index = 0
                while axes_changed:
                    lowest_bit_set = low_bit_order(axes_changed)
                    axis_index = axis_index + lowest_bit_set
                    if debug_output:
                        print(" AXIS: ", axis_index, end="")
                    if (axis_map):
                        axis = axis_map[axis_index]
                        if debug_output:
                            print("(", axis,":", AXIS_NAMES[axis], ")", end="")
                        
                    if debug_output:
                        print (axes[axis_index], end="")
                    axis_motion_list.append(axis)
                    axis_motion_list.append(axes[axis_index])

                    axes_changed = axes_changed >> (lowest_bit_set + 1)
                    axis_index = axis_index + 1                        
                if debug_output:
                    print("")
                Bridge.notify("joy_axis_motion", axis_motion_list)

    except Exception as e:
        if debug_output:
            print("Error reading joystick state:", e)
            print(state)
    
    time.sleep(0.025) 

def set_debug_output(enable: int):
    global debug_output
    debug_output = enable

Bridge.provide("debug_output", set_debug_output)
App.run(user_loop=loop)
