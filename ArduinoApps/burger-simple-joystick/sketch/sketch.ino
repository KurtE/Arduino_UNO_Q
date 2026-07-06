#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <vector>

#include "Arduino_RouterBridge.h"
#include <Dynamixel2Arduino.h>
#include "memorySerial.h"


// Dynamixel defines.
#define DXL_SERIAL Serial1
#define LEFT_WHEEL_ID 1
#define RIGHT_WHEEL_ID 2


uint32_t last_led_change_time_ms = 0;
char buffer[128];

// Simple memory Serial class
memorySerial MSerial;


#define STACK_SIZE 1024
#define PRIORITY 7

typedef struct {
  uint16_t    value;
  const char  *name;  
} value_to_name_t;

enum { JOY_LEFT_X = 0,
       JOY_LEFT_Y,
       JOY_RIGHT_X,
       JOY_RIGHT_Y = 5,
       JOY_RIGHT_TRIGGER = 9,
       JOY_LEFT_TRIGGER
};

enum {
    JOY_BTN_CROSS = 305,
    JOY_BTN_CIRCLE = 306,
    JOY_BTN_TRIANGLE = 307,
    JOY_BTN_SQUARE = 304,
    JOY_BTN_L2 = 310,
    JOY_BTN_R2_ = 311,
    JOY_BTN_SHARE = 312,
    JOY_BTN_OPTIONS = 313,
    JOY_BTN_PS_XB = 316,
    JOY_BTN_VIEW = 314,
    JOY_BTN_MENU = 315,
    JOY_BTN_LTSTICK = 317,
    JOY_BTN_RTSTICK = 318,
    //JOY_BTN_SHARE = 158,
    JOY_BTN_GUID = 172,

};

static const value_to_name_t axis_names[] = {
    {0, "lx"},
    {1, "ly"},
    {2, "rx"},
    {5, "ry"},
    {3, "L2"},
    {4, "R2"},
    {9, "R2/Rt"},
    {10, "L2/Lt"},
    {16, "dpad_x"},
    {17, "dpad_y"}
};


static const value_to_name_t button_names[] = {
    {305, "cross/B"},
    {306, "circle"},
    {307, "triangle/X"},
    {304, "square/A"},
    {308, "l1/Y"},
    {309, "r1/rs"},
    {310, "l2/ls"},
    {311, "r2_btn"},
    {312, "share"},
    {313, "options"},
    {316, "ps/XB"},
    {314, "l3/view"},
    {315, "r3/options"},
    {317, "touchpad/Stl Lt Bth"},
    {318, "Stick Rt Btn"},
    {158, "share"},
    {172, "guide"}
};


// Dynamixel variables.
const int DXL_DIR_PIN = 2;  // DYNAMIXEL Shield DIR PIN
const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
#define DXL_SERIAL Serial1
uint8_t servos_found = 0xff;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

int16_t cur_wheel_velocity = 0;
volatile int16_t new_wheel_velocity = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Bridge.begin();
  Serial.begin(115200);
  
  //  while (!Serial && millis() < 5000) {}
  delay(5000);
  Serial.println("\n*** Joystick test program starting ***");
  Serial.flush();

  // start our memory Serial object to buffer between the two threads.
  MSerial.begin();
  
  Bridge.provide("joy_button_down", joy_button_down);
  Bridge.provide("joy_button_up", joy_button_up);
  Bridge.provide("joy_axis_motion", joy_axis_motion);
  //Bridge.provide("joy_hat_motion", joy_hat_motion);

  // Start up Dynamixel support
  // Set Port baudrate to 1MBs. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  Serial.print("After dxl.begin\n");
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  Serial.println("Set protocol");
 
  
}


void loop() {
  if ((millis() - last_led_change_time_ms) > 500) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    last_led_change_time_ms = millis();
  }

  uint8_t mbuf[64];
  int cbRead;
  while ((cbRead = MSerial.read(mbuf, sizeof(mbuf))) > 0) {
    Serial.write(mbuf, cbRead);
  }

  
  if (servos_found == 0xff) {
    FindServos();
  }

  if ((new_wheel_velocity != cur_wheel_velocity) && (servos_found == 3)) {
      Serial.println("change Velocity");
      cur_wheel_velocity = new_wheel_velocity;
      dxl.setGoalVelocity(LEFT_WHEEL_ID, cur_wheel_velocity);
      dxl.setGoalVelocity(RIGHT_WHEEL_ID, cur_wheel_velocity);
  }
}

const char *map_button_to_name(int btn) {
  for(int i = 0; i < sizeof(button_names)/sizeof(button_names[0]); i++) {
    if (button_names[i].value == btn) return button_names[i].name;   
  }
  return nullptr;
}

void joy_button_down(int btn) {
  MSerial.print("BDN: ");
  MSerial.print(btn);
  const char *btn_name = map_button_to_name(btn);
  if (btn_name != nullptr) {
    MSerial.print(" (");
    MSerial.print(btn_name);
    MSerial.print(")");
  }
  MSerial.println();

  if (btn == JOY_BTN_TRIANGLE) {
    servos_found = 0xff;
  }

}

void joy_button_up(int btn) {
  MSerial.print("BUP: ");
  MSerial.print(btn);
  const char *btn_name = map_button_to_name(btn);
  if (btn_name != nullptr) {
    MSerial.print(" (");
    MSerial.print(btn_name);
    MSerial.print(")");
  }
  MSerial.println();

  if (btn == 3) {
    servos_found = 0xff;
  }

}

const char *map_axis_to_name(int axis) {
  for(int i = 0; i < sizeof(axis_names)/sizeof(axis_names[0]); i++) {
    if (axis_names[i].value == axis) return axis_names[i].name;   
  }
  return nullptr;
}

void joy_axis_motion(std::vector<int> motions) {
  MSerial.print("Axis:");
  for (int i = 0; i < motions.size(); i += 2) {
    MSerial.print(" ");
    MSerial.print(motions[i]);
    const char * axis_name = map_axis_to_name(motions[i]);
    if (axis_name != nullptr) {
      MSerial.print(" (");
      MSerial.print(axis_name);
      MSerial.print(")");
    }
    MSerial.print(":");
    MSerial.print(motions[i + 1]);

    if (motions[i] == JOY_LEFT_Y) {
      // values are +-32K and Dynamixel are +-1K... 
      new_wheel_velocity = motions[i+1] / 32;
    }
  }
  MSerial.println();
}

//void joy_hat_motion(int x, int y) {
//  sprintf(buffer, "Hat: %d %d\n", x, y);
//  Serial.print(buffer);
//}

DYNAMIXEL::InfoFromPing_t ping_info[32];

void FindServos(void) {
  servos_found = 0;
  Serial.println("  Try Protocol 2 - broadcast ping: ");
  Serial.flush();  // flush it as ping may take awhile...
  if (uint8_t count_pinged = dxl.ping(DXL_BROADCAST_ID, ping_info,
                                      sizeof(ping_info) / sizeof(ping_info[0]))) {
    //Serial.print("Detected Dynamixel : \n");
    Serial.println("Detected Dynamixel :");
    for (int i = 0; i < count_pinged; i++) {
      sprintf(buffer, "    %u, Model:%d, Ver:%d\n", ping_info[i].id, ping_info[i].model_number, ping_info[i].firmware_version);
      Serial.print(buffer);
      if (ping_info[i].id == LEFT_WHEEL_ID) {
        Serial.println("Found Left wheel");
        dxl.torqueOff(LEFT_WHEEL_ID);
        dxl.setOperatingMode(LEFT_WHEEL_ID, OP_VELOCITY);
        dxl.torqueOn(LEFT_WHEEL_ID);
        servos_found |= 1;
      } else if (ping_info[i].id == RIGHT_WHEEL_ID) {
        dxl.torqueOff(RIGHT_WHEEL_ID);
        dxl.setOperatingMode(RIGHT_WHEEL_ID, OP_VELOCITY);
        dxl.torqueOn(RIGHT_WHEEL_ID);
        Serial.println("Found right wheel");
        servos_found |= 2;
      }
    }
  } else {
    Serial.print("Broadcast returned no items : ");
    Serial.println(dxl.getLastLibErrCode());
    //printk("Broadcast returned no items: %d\n", dxl.getLastLibErrCode());
  }
}
