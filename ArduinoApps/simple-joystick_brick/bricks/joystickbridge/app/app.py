# app.py
from joystick_helper import Joystick
import threading
from flask import Flask, jsonify, request

app = Flask(__name__)


js = Joystick()
threading.Thread(target=js.loop, daemon=True).start()

# ----------------------------------------------------
# Joystick metadata
# ----------------------------------------------------
@app.route("/joystick/name")
def js_name():
    return jsonify({"name": js.get_name()})

@app.route("/joystick/connected")
def js_connected():
    return jsonify({"connected": js.get_connected()})

@app.route("/joystick/num_axes")
def js_num_axes():
    return jsonify({"num_axes": js.get_num_axes()})

@app.route("/joystick/num_buttons")
def js_num_buttons():
    return jsonify({"num_buttons": js.get_num_buttons()})

@app.route("/joystick/axis_map")
def js_axis_map():
    return jsonify({"axis_map": js.get_axis_map()})

@app.route("/joystick/button_map")
def js_button_map():
    return jsonify({"button_map": js.get_button_map()})

# ----------------------------------------------------
# Joystick live state
# ----------------------------------------------------
@app.route("/joystick/axes")
def js_axes():
    return jsonify(js.get_axis_values())

@app.route("/joystick/buttons")
def js_buttons():
    return jsonify(js.get_button_values())

@app.route("/joystick/state")
def js_state():
    return jsonify(js.get_state())

@app.route("/joystick/spidev_output")
def js_enable_spidev_output():
    # Get the query parameter as string
    flag_str = request.args.get("enable", "").lower()

    # Convert to boolean
    if flag_str in ["true", "1", "yes"]:
        enable = True
    elif flag_str in ["false", "0", "no"]:    
        enable = False
    else:
        enable = None

    return jsonify(js.spidev_output(enable))

if __name__ == "__main__":
    app.run(debug=True)
