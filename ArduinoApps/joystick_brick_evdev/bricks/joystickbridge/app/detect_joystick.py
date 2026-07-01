import os
from evdev import InputDevice

def find_ps4_event():
    base = "/dev/input"

    # Scan all event devices
    for name in os.listdir(base):
        if not name.startswith("event"):
            continue

        path = os.path.join(base, name)

        try:
            dev = InputDevice(path)
            dev_name = dev.name.lower()

            # Match any PS4 controller naming pattern
            if ("sony" in dev_name or
                "wireless controller" in dev_name or
                "dualshock" in dev_name or
                "playstation" in dev_name):

                return path

        except Exception:
            continue

    return None

