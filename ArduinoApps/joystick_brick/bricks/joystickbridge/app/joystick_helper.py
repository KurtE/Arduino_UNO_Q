# joystick.py
import os
import struct
import fcntl
import threading

JSIOCGNAME = 0x80006a13
JSIOCGAXES = 0x80016a11
JSIOCGBUTTONS = 0x80016a12
JSIOCGAXMAP = 0x80406a32
JSIOCGBTNMAP = 0x80406a34

class Joystick:
    def __init__(self, device="/dev/input/js0"):
        self.device = device
        self.fd = None

        self.axis_map = []
        self.button_map = []
        self.axis_states = {}
        self.button_states = {}

        self.num_axes = 0
        self.num_buttons = 0

        self.running = False
        self.lock = threading.Lock()

    # ----------------------------------------------------
    # Initialization
    # ----------------------------------------------------
    def open(self):
        if not os.path.exists(self.device):
            raise FileNotFoundError(f"Joystick device {self.device} not found")

        self.fd = open(self.device, "rb")

        # Read name
        buf = bytearray(64)
        fcntl.ioctl(self.fd, JSIOCGNAME, buf)
        self.name = buf.rstrip(b"\x00").decode("utf-8", errors="ignore")

        # Axes
        buf = bytearray(1)
        fcntl.ioctl(self.fd, JSIOCGAXES, buf)
        self.num_axes = buf[0]

        # Buttons
        buf = bytearray(1)
        fcntl.ioctl(self.fd, JSIOCGBUTTONS, buf)
        self.num_buttons = buf[0]

        # Axis map
        buf = bytearray(0x40)
        fcntl.ioctl(self.fd, JSIOCGAXMAP, buf)
        self.axis_map = list(buf[:self.num_axes])

        # Button map
        buf = bytearray(0x40)
        fcntl.ioctl(self.fd, JSIOCGBTNMAP, buf)
        self.button_map = list(buf[:self.num_buttons])

        # Initialize states
        with self.lock:
            for a in self.axis_map:
                self.axis_states[a] = 0.0
            for b in self.button_map:
                self.button_states[b] = 0

    # ----------------------------------------------------
    # Background event loop
    # ----------------------------------------------------
    def loop(self):
        self.running = True
        self.open()

        while self.running:
            ev = self.fd.read(8)
            if len(ev) < 8:
                continue

            time, value, etype, number = struct.unpack("IhBB", ev)
            etype &= 0x7F  # strip init flag

            with self.lock:
                if etype == 0x01:  # button
                    if number < len(self.button_map):
                        btn = self.button_map[number]
                        self.button_states[btn] = 1 if value else 0

                elif etype == 0x02:  # axis
                    if number < len(self.axis_map):
                        axis = self.axis_map[number]
                        self.axis_states[axis] = value

    # ----------------------------------------------------
    # Public API (the functions you asked for)
    # ----------------------------------------------------
    def get_num_axes(self):
        return self.num_axes

    def get_num_buttons(self):
        return self.num_buttons

    def get_axis_map(self):
        return list(self.axis_map)

    def get_button_map(self):
        return list(self.button_map)

    def get_axis_values(self):
        with self.lock:
            return dict(self.axis_states)

    def get_button_values(self):
        with self.lock:
            return dict(self.button_states)

    def get_state(self):
        with self.lock:
            return {
                "axes": dict(self.axis_states),
                "buttons": dict(self.button_states)
            }
