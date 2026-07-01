# main.py
import time
import joystickbridge as joystick

# ----------------------------------------------------
# Axis + Button Names for js0 (PS4-style)
# ----------------------------------------------------
AXIS_NAMES = {
    0: "lx",
    1: "ly",

    # Right stick (USB mapping)
    2: "rx",
    5: "ry",

    # Bluetooth fallback
    3: "rx",
    4: "ry",

    # D-pad (varies by driver)
    10: "dpad_x",
    11: "dpad_y",
    16: "dpad_x",
    17: "dpad_y",
}

BUTTON_NAMES = {
    48: "square",
    49: "circle",
    50: "L1",          # we will discover this one
    51: "triangle",
    52: "cross",
    54: "start",

    # Add more as we observe them
}


# ----------------------------------------------------
# Inspector Class (Pretty Printing)
# ----------------------------------------------------
class Inspector:
    def __init__(self):
        self.last_axes = {}
        self.last_buttons = {}

    def print_changes(self, axes, buttons):
        changed = False

        # -------------------------
        # AXES
        # -------------------------
        for raw_code, value in axes.items():
            code = int(raw_code)

            # Use name if known, else fallback
            name = AXIS_NAMES.get(code, f"axis_{code}")

            old = self.last_axes.get(code)
            if old is None or old != value:
                delta = 0 if old is None else value - old
                print(f"Axis {name:<8} {value:>7.3f} (Δ {delta:+7.3f})")
                self.last_axes[code] = value
                changed = True

        # -------------------------
        # BUTTONS
        # -------------------------
        for raw_code, value in buttons.items():
            code = int(raw_code)
        
            name = BUTTON_NAMES.get(code, f"btn{code}")
        
            old = self.last_buttons.get(code)
            if old is None or old != value:
                print(f"Button {name:<10} {value}")
                self.last_buttons[code] = value
                changed = True

        return changed


# ----------------------------------------------------
# MAIN
# ----------------------------------------------------
def main():
    print("=== Joystick Brick Test ===")

    # Metadata
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

    print("\n=== Live Joystick State ===")

    inspector = Inspector()

    while True:
        try:
            state = joystick.getState()
            axes = state.get("axes", {})
            buttons = state.get("buttons", {})

            if inspector.print_changes(axes, buttons):
                print()

        except Exception as e:
            print("Error reading joystick state:", e)

        time.sleep(0.05)  # ~20 Hz update rate


if __name__ == "__main__":
    main()
