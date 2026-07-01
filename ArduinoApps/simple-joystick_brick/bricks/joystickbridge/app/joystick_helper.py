
# joystick.py
import os
import struct
import fcntl
import threading
import spidev
import time

JSIOCGNAME = 0x80006a13
JSIOCGAXES = 0x80016a11
JSIOCGBUTTONS = 0x80016a12
JSIOCGAXMAP = 0x80406a32
JSIOCGBTNMAP = 0x80406a34

class Joystick:
    def __init__(self, device="/dev/input/js0"):
        self.device = device
        self.name = None
        self.fd = None

        self.axis_map = []
        self.button_map = []
        self.axis_states = {}
        self.button_states = {}

        self.axis_changed = False
        self.buttons_changed = False

        self.num_axes = 0
        self.num_buttons = 0

        self.running = False
        self.joystick_connected = False
        self.output_to_spidev = True
        self.lock = threading.Lock()

        self.spi = None


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
        fmt = "H" * self.num_buttons
        
        try:
            self.button_map = list(struct.unpack(fmt, buf[:self.num_buttons * 2]))
            
        except Exception as e:
            print("An error occurred button_map:", str(e))
            print("BUF:", buf, "Num Btns:", self.num_buttons)

        # Initialize states
        with self.lock:
            for a in self.axis_map:
                self.axis_states[a] = 0
            for b in self.button_map:
                self.button_states[b] = 0

        # open SPIDEV
        spi = spidev.SpiDev()
        spi.open(0, 0)
        spi.max_speed_hz = 5000000
        spi.mode = 0
        spi.bits_per_word = 8
        self.spi = spi

    # ----------------------------------------------------
    # detect if joystick is attached.
    # ----------------------------------------------------
    
    # ----------------------------------------------------
    # Background event loop
    # ----------------------------------------------------
    def loop(self):
        self.running = True
        #self.open()
        self.joystick_connected = False
        
        while self.running:
            if not self.joystick_connected:
                print("Not Connected: ", self.device)
                
                while True: 
                    if os.path.exists(self.device) == True:
                        break;
                    time.sleep(0.25)
                    
                self.open()
                self.joystick_connected = True
                print("Joystick now Connected")
                
            try:
                ev = self.fd.read(8)
                if len(ev) < 8:
                    continue
    
                etime, value, etype, number = struct.unpack("IhBB", ev)
                etype &= 0x7F  # strip init flag
                with self.lock:
                    # optionally output to spidev
                    if self.output_to_spidev:
                        self.spi.xfer2(ev)

                    if etype == 0x01:  # button
                        if number < len(self.button_map):
                            btn = self.button_map[number]
                            self.button_states[btn] = 1 if value else 0
                            if self.buttons_changed == False:
                                self.buttons_changed = btn
                            elif self.buttons_changed != btn:
                                self.buttons_changed = True
                    
                    elif etype == 0x02:  # axis
                        if number < len(self.axis_map):
                            axis = self.axis_map[number]
                            self.axis_states[axis] = value
                            if self.axis_changed == False:
                                self.axis_changed = axis
                            elif self.axis_changed != axis:
                                self.axis_changed = True
                            
            except Exception as e:
                print("An error occurred:", str(e))
                break
                
    # ----------------------------------------------------
    # Public API (the functions you asked for)
    # ----------------------------------------------------
    def get_connected(self):
        return self.joystick_connected

    def get_name(self):
        return self.name
        
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
            ret = {
                "axes_changed": self.axis_changed,
                "axes": dict(self.axis_states),
                "buttons_changed": self.buttons_changed,
                "buttons": dict(self.button_states),
            }
            self.axis_changed = False
            self.buttons_changed = False
            return ret;

    def spidev_output(self, enable):
        print("spidev_output:", enable)
        if enable != None:
            self.output_to_spidev = enable
        return self.output_to_spidev
        