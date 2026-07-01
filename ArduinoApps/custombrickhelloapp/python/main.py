import time

from arduino.app_utils import App
import hello

print("Hello from the App script!")


def loop():
    hello.say_hello()
    time.sleep(1)


App.run(user_loop=loop)
