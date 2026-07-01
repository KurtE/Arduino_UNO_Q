#!/usr/bin/env python3
import os
import struct
import sys
import time

JOYSTICK_PATH = "/dev/input/js0"

# Joystick event format:
# struct js_event {
#     uint32_t time;     // event timestamp in milliseconds
#     int16_t  value;    // value
#     uint8_t  type;     // event type
#     uint8_t  number;   // axis/button number
# };
EVENT_FORMAT = "IhBB"     # matches the C structure
EVENT_SIZE = struct.calcsize(EVENT_FORMAT)

# Event type bit flags
JS_EVENT_BUTTON = 0x01
JS_EVENT_AXIS   = 0x02
JS_EVENT_INIT   = 0x80  # Initial state events at startup

def open_joystick(path="/dev/input/js0"):
    """Open the joystick device safely."""
    if not os.path.exists(path):
        print(f"Error: joystick device {path} not found.")
        sys.exit(1)
    try:
        return open(path, "rb")
    except PermissionError:
        print(f"Permission denied opening {path}. Try running with correct permissions.")
        sys.exit(1)
    except OSError as e:
        print(f"Failed to open joystick device: {e}")
        sys.exit(1)

def main():
    keyboard_abort = False
    while True:
        # lets wait for joystick to connect
        if not os.path.exists(JOYSTICK_PATH):
            print("Waiting for Joystick")

            while True:
                if os.path.exists(JOYSTICK_PATH):
                    break
                time.sleep(0.25)
    
        js = open_joystick()

        print("Reading joystick events. Press Ctrl+C to exit.")
        try:
            while True:
                try:
                    data = js.read(EVENT_SIZE)
                except Exception as e:
                    # Handle unexpected errors safely
                    print("An error occurred:", str(e))
                    print("Breaking out of loop due to exception.")
                    break

                if len(data) != EVENT_SIZE:
                    print("Incomplete event read. Device may have disconnected.")
                    break

                js_time, value, etype, number = struct.unpack(EVENT_FORMAT, data)

                # Filter out initialization events unless needed
                is_init = bool(etype & JS_EVENT_INIT)
                etype = etype & ~JS_EVENT_INIT

                if etype == JS_EVENT_BUTTON:
                    state = "pressed" if value else "released"
                    print(f"[{js_time} ms] Button {number} {state} (init={is_init})")

                elif etype == JS_EVENT_AXIS:
                    print(f"[{js_time} ms] Axis {number} value={value} (init={is_init})")

                # Unknown event type (rare)
                else:
                    print(f"[{js_time} ms] Unknown event type {etype}")

        except KeyboardInterrupt:
            print("\nExiting...")
            keyboard_abort = True
        finally:
            js.close()

        if keyboard_abort:
            break

if __name__ == "__main__":
    main()
