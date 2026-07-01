import time
import joystickbridge as joystick

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

def main():
    print("=== Joystick Brick Test ===")
    print(">> Waiting for Joystick")
    joystick_connected = False

    try:
        while True:
            connected = joystick.getConnected()
            is_connected = connected['connected']
            print ("Connected: ", is_connected, type(is_connected))
            if is_connected == True:
                break
            print(".")
            time.sleep(2.5)

    except Exception as e:
        print("Error waing for joystick:", e)
        return
        
    print(">> Joystick connected")
    
    try:
        name = joystick.getName()
        print("Joystick Name:", name)

        num_axes = joystick.getNumAxes()
        print("Number of Axes:", num_axes)

        num_buttons = joystick.getNumButtons()
        print("Number of Buttons:", num_buttons)

        axis_map = joystick.getAxisMap()
        print("RAW axis_map:", axis_map)

        button_map = joystick.getButtonMap()
        print("RAW button_map:", button_map)

    except Exception as e:
        print("Error reading joystick metadata:", e)
        return
    
    # Live loop
    spidev_enable = True
    
    while True:
        # Metadata
        try:
            state = joystick.getState()
            axes_changed = state["axes_changed"]
            buttons_changed = state["buttons_changed"]
                
            if axes_changed != False or type(axes_changed) != bool or buttons_changed != False:
                print("--------------")

                if buttons_changed != False:
                    buttons = {int(k): v for k, v in state.get("buttons", {}).items()}
                    print("Buttons Changed:", buttons_changed, end="")
                    if buttons_changed == True:
                        print(buttons)
                    else:    
                        print(" BTN:",BUTTON_NAMES[buttons_changed], end="" )
                        if (buttons[buttons_changed] != 0):
                            print(" Pressed");

                            #experiment to see if I can turn on/and off the spidev interface.
                            if buttons_changed == 315: #little one right and bellow X button
                                spidev_enable = not spidev_enable
                                joystick.setEnableSpidevOutput(spidev_enable)
                                
                        else:
                            print(" Released")

                if type(axes_changed) == bool:
                    if axes_changed == True:
                        axes = {int(k): v for k, v in state.get("axes", {}).items()}
                        print("Axes Changed:", axes_changed, end="")
                        print(axes)
                else:
                    axes = {int(k): v for k, v in state.get("axes", {}).items()}
                    print("Axes Changed:", axes_changed, end="")
                    print(" Axis: ", AXIS_NAMES[axes_changed], axes[axes_changed])
    
            #axis_values = [axes.get(code, 0) for code in AXIS_ORDER]
            #button_values = [buttons.get(code, 0) for code in BUTTON_ORDER]
            #if inspector.print_changes(axes, buttons):
            #    print("AX:", axis_values)
            #    print("BT:", button_values)
            #    print()
    
        except Exception as e:
            print("Error reading joystick state:", e)
    
        time.sleep(0.05) 


if __name__ == "__main__":
    main()
