import time
import os

from arduino.app_utils import *

# Specify the folder path
folder_path = r"/sys/class/thermal"

def get_mpu_temp(thermal_name: str) -> float:
    """
    Obtains the current value of the CPU temperature.
    :returns: Current value of the CPU temperature if successful, zero value otherwise.
    :rtype: float
    """
    # Initialize the result.
    result = 0.0
    # The first line in this file holds the CPU temperature as an integer times 1000.
    # Read the first line and remove the newline character at the end of the string.
    full_path = os.path.join(folder_path, thermal_name)
    thermal_file = os.path.join(full_path, "temp")
    result = 0.0;
    if os.path.isfile(thermal_file):
        with open(thermal_file) as f:
            line = f.readline().strip()
        # Test if the string is an integer as expected.
        if line.isdigit():
            # Convert the string with the CPU temperature to a float in degrees Celsius.
            result = float(line) / 1000
    
    # Give the result back to the caller.
    return result

def get_mpu_type(thermal_name: str) -> str:
    """
    Obtains the current value of the CPU temperature.
    :returns: Current value of the CPU temperature if successful, zero value otherwise.
    :rtype: float
    """
    # Initialize the result.
    result = 0.0
    # The first line in this file holds the CPU temperature as an integer times 1000.
    # Read the first line and remove the newline character at the end of the string.
    full_path = os.path.join(folder_path, thermal_name)
    thermal_file = os.path.join(full_path, "type")
    result = ""
    if os.path.isfile(thermal_file):
        with open(thermal_file) as f:
            result = f.readline().strip()
    
    # Give the result back to the caller.
    # print("file: ", thermal_file, "result: ", result)
    return result
    
print("MPU/MCU temperature test program!")


def loop():
    """This function is called repeatedly by the App framework."""
    # You can replace this with any code you want your App to run repeatedly.
    #print('Current CPU temperature is {:.2f} degrees Celsius.'.format(get_cpu_temp()))
    #print('\t {:.2f} degrees Farhrenheit.'.format(get_cpu_temp()*1.8 + 32))
    time.sleep(10)

# See: https://docs.arduino.cc/software/app-lab/tutorials/getting-started/#app-run

Bridge.provide("get_mpu_temp", get_mpu_temp)
Bridge.provide("get_mpu_type", get_mpu_type)

#lets try to list all of the thermal objects

# List all files (excluding directories)
files = [f for f in os.listdir(folder_path) if f.startswith("thermal_")]

print("Files in folder:")
for file in files:
    full_path = os.path.join(folder_path, file)
    with open(os.path.join(full_path, "type")) as f:
        temp_type = f.readline().strip()
    
    result = 0.0;
    with open(os.path.join(full_path, "temp")) as f:
        line = f.readline().strip()
        # Test if the string is an integer as expected.
        if line.isdigit():
            # Convert the string with the CPU temperature to a float in degrees Celsius.
            result = float(line) / 1000

    print("file: ", file, " type:", temp_type, "Current Temp C:", result)
    
App.run(user_loop=loop)
