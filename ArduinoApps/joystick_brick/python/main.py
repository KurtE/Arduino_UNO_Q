import time
import joystickbridge as joystick

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
        print("Axis Map:", axis_map)

        button_map = joystick.getButtonMap()
        print("Button Map:", button_map)

    except Exception as e:
        print("Error reading joystick metadata:", e)
        return

    print("\n=== Live Joystick State ===")

    # Live loop
    while True:
        try:
            state = joystick.getState()
            axes = state.get("axes", {})
            buttons = state.get("buttons", {})

            print("\nAxes:")
            for k, v in axes.items():
                print(f"  Axis {k}: {v:.3f}")

            print("Buttons:")
            for k, v in buttons.items():
                print(f"  Button {k}: {v}")

        except Exception as e:
            print("Error reading joystick state:", e)

        time.sleep(0.1)  # 10 Hz update rate


if __name__ == "__main__":
    main()
