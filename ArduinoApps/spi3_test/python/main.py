import time
#import subprocess
#import sys
import spidev

#try:
#    import spidev
#except ModuleNotFoundError:
#print("Installing spidev package...")
#subprocess.check_call([sys.executable, "-m", "pip", "install", "spidev", "--break-system-packages"])
#print("spidev package installed successfully!")
#import spidev

from arduino.app_utils import App

print("Hello world!")


def loop():
    """This function is called repeatedly by the App framework."""
    # You can replace this with any code you want your App to run repeatedly.
    spi = spidev.SpiDev()
    spi.open_path("/dev/spidev0.0")
    spi.max_speed_hz = 20000000
    spi.readbytes(512)
    time.sleep(10)


# See: https://docs.arduino.cc/software/app-lab/tutorials/getting-started/#app-run
App.run(user_loop=loop)
