//=============================================================================
// Zephyr Pin test using my digitalWriteFast library for Zephyr (STM32)
// It uses access to the pins using Pin Names.
//
// This sketch is setup to set all pins (minus thouse exceluded) into either
// INPUT_PULUP or INPUT_PULLDOWN and then scans all of those pins to see if
// they change state and if so prints out the new state.
//
// This allow you when in PULL Up mode to use a jumper wire to ground and try
// touching the pins in question.  of in Pull Down mode connect pins up to
// 3.3v.  When the pins change state, the code prints out the logical 
// Pin name for that pin(s) and if the is in the Arduino Pin table it
// also prints out the Arduino pin number.  Example on Portenta H7,
// I touched logical pin 5 and it printed out:
// PC_6(5) val=0,
// Some boards like Portenta H7 Arduino tables do not currently
// contain all of pins on the high density connectors, which if
// connected up to one of their Carrier boards, may print out 
// like: PI_5 val=0,
//
//=============================================================================
#include <digitalWriteFast_zephyr.h>
//#define PRINT_DEBUG_PIN_ENUM

const uint8_t count_pin_names = PinName::PX_COUNT;
uint8_t pin_test_mode = 1;

// This one is setup currently for Portenta H7
uint8_t pinLast[count_pin_names] = {0};
#if 0
  // clang-format off
    0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff, //PA_0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,  
    0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, //PB_0
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, //PC_0
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, //PD_0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, //PE_0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //PF_0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, //PG_0
    0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, //PH_9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //PI_0
    0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, //PJ_0
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  //PK_0
  // clang-format on
};
#endif

#if defined(ARDUINO_UNO_Q)
static const uint16_t pinname_exclude_list[] = {
  // LEDS PH_10=15, LED_MATRIX=PF0-10, SPI RDR=PG_13, Analog switch=PA_2, BOOT0 PH_3
  0b0000000000000100, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000,  // GPIOA-D
  0b0000000000000000, 0b0000011111111111, 0b1111111111111111, 0b1111110000001000  // GPIOE-PH
};

#elif defined(ARDUINO_PORTENTA_H7_M7)
static const uint16_t pinname_exclude_list[11] = {
  // LEDS PH_10=15, LED_MATRIX=PF0-10, SPI RDR=PG_13, Analog switch=PA_2, BOOT0 PH_3
  0b1000000010101110, 0b0011110000100011, 0b0101111100110011, 0b1111111100000111, // GPIOA-D
  0b1111111111110011, 0b1111111111111111, 0b1011100101110111, 0b0000000000111111, // GPIOE-PH
  0b0001100000000000, 0b1111000000111111, 0b1111111111111101                              // GPIOI-K
};
#define PROMPT_FOR_ARDUINO_PINS_ONLY

#elif defined(ARDUINO_GIGA)
static const uint16_t pinname_exclude_list[] = {
  0b0000000000000000, 0b0000000000000000, 0b0010000000000000, 0b0000000000000000,  // GPIOA-D
  0b0000000000001000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000,  // GPIOE-PH
  0b0001000000000000, 0b0010000000000000, 0b1111111100000000                      // GPIOI-K
};
#elif defined(ARDUINO_NICLA_VISION)
static const uint16_t pinname_exclude_list[] = {
  0b0000000000000000, 0b0000000000000000, 0b0010000000000000, 0b0000000000000000,  // GPIOA-D
  0b0000000000001000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000,  // GPIOE-PH
  0b0001000000000000, 0b0010000000000000, 0b1111111100000000                      // GPIOI-K
};
#else
static const uint16_t pinname_exclude_list[11] = {0};
#define PROMPT_FOR_ARDUINO_PINS_ONLY
#endif

//#if DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios) > 0
//uint16_t PINS_COUNT = DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios);
const uint8_t PINS_COUNT = sizeof(pinLast);
//#endif

int index_pinLast_used = -1;

bool pins_changed[count_pin_names];

extern void allPinTest();

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}

  Serial.println("\n*** Pin High/Low Test for Zephyr STM32 Boards **");
  
  // default to only Arduino Pins.
  #ifdef PROMPT_FOR_ARDUINO_PINS_ONLY
  Serial.println("Enable only Arduino defined pines? (Y/N) (Defaults No in 10 seconds)");

  uint32_t start_time = millis();
  while (!Serial.available() && (millis() - start_time) < 10000) {}
  int ch = Serial.read();
  if (ch == 'y' || ch == 'Y') 
  #endif
  {
    Serial.println("*** Only Arduino pins enabled ***");
    enable_only_arduino_pins();
  }

  exclude_pins();
  // find the last pin name used
  for (index_pinLast_used = (sizeof(pinname_exclude_list)/sizeof(pinname_exclude_list[0]))*16 - 1; index_pinLast_used >= 0; index_pinLast_used--) {
    if (pinLast[index_pinLast_used] == 0) break;
  }

  

  Serial.print("NUM_OF_DIGITAL_PINS: ");
  Serial.println(NUM_OF_DIGITAL_PINS, DEC);
  Serial.println("Pins included in test:");
  int index_first_in_series = -1;
  for (int i = 0; i <= index_pinLast_used; i++) {
    if (pinLast[i] != 0xff) {
      if (index_first_in_series == -1) index_first_in_series = i;
    } else {
      // end of series
      if (index_first_in_series != -1) {
        Serial.print(" ");
        Serial.print(pinNameToStr(index_first_in_series));
        if (index_first_in_series != (i - 1)) {
          Serial.print("-");
          Serial.print(pinNameToStr(i - 1));
        }
        index_first_in_series = -1;
      }
    }
  }
  Serial.println();
  Serial.flush();

}

void loop() {
  allPinTest();
}

void enable_only_arduino_pins() {
  memset(pinLast, 0xff, sizeof(pinLast));

  for (uint8_t pin_num = 0; pin_num < NUM_OF_DIGITAL_PINS; pin_num++) {
    PinName pn = mapPinToPinName(pin_num);
    if (pn != (PinName)0xff) pinLast[(uint8_t)pn] = 0;
  }

}

void exclude_pins() {
  int pin_name_index = 0;
  for (uint8_t i = 0; i < (sizeof(pinname_exclude_list)/sizeof(pinname_exclude_list[0])); i++) {
    uint16_t exclude_pins = pinname_exclude_list[i];
    for (uint8_t j = 0; j < 16; j++) {
      if(exclude_pins & 1) pinLast[pin_name_index] = 0xff;
      pin_name_index++;
      exclude_pins >>= 1;
    }
  }
}

void allPinTest() {
  int ii;
  for (ii=0; ii <= index_pinLast_used; ii++) pins_changed[ii] = false;

  Serial.print("PULLUP Start Vals:\n  ");
  Serial.print("PULLUP :: TEST to GND\n  ");
  for (ii = 0; ii <= index_pinLast_used; ii++) {
    PinName pin_name = (PinName)ii;
    if (pinLast[ii] != 0xff) {
      if ((ii == 0) || (pinLast[ii - 1] == 0xff)) {
        Serial.print("\n(");
        Serial.print(pinNameToStr(ii));
        Serial.print(") ");
        Serial.flush();
      }
      pinMode(pin_name, INPUT_PULLUP);
      #ifdef PRINT_DEBUG_PIN_ENUM
      Serial.print(ii, HEX); Serial.flush();
      #endif
      delayMicroseconds(5);
      pinLast[ii] = digitalReadFast(pin_name);
      #ifdef PRINT_DEBUG_PIN_ENUM
      Serial.print(":"); Serial.println(pinLast[ii]); Serial.flush();
      #endif      
      if (!pinLast[ii]) {
        Serial.print("\nd#=");
        Serial.print(pinNameToStr(ii));
        Serial.print(" val=");
      }
      Serial.print(pinLast[ii]);
      Serial.print(',');
    }
  }
  Serial.println();
  Serial.println();
  print_all_gpio_regs();
  while (1) {
    uint32_t jj, dd = 0, cc = 0;
    cc = 0;
    for (ii = 0; ii <= index_pinLast_used; ii++) {
      PinName pin_name = (PinName)ii;
      if (pinLast[ii] != 0xff) {
        jj = digitalReadFast(pin_name);
        if (jj != pinLast[ii]) {
          pins_changed[ii] = true;
          dd = 1;
          cc++;
          pinLast[ii] = jj;
          Serial.print(pinNameToStr(ii));
          // See if this name maps to Arduino pin number
          uint8_t arduino_pin_number = mapPinNameToPin(pin_name);
          if (arduino_pin_number != 0xff) {
            Serial.print("(");
            Serial.print(arduino_pin_number);
            Serial.print(")");
          }
          if (pinLast[ii]) Serial.print("\t");
          Serial.print(" val=");
          Serial.print(pinLast[ii]);
          Serial.print(',');
        }
      }
    }
    if (dd) {
      dd = 0;
      Serial.println();
      delay(50);
    }

    if (Serial.available()) {
      while (Serial.available()) Serial.read();
    
      Serial.println("Pins that were touched: ");
      bool changed_found = false;
      for(ii=0; ii <= index_pinLast_used; ii++) {
        if (pins_changed[ii]) {
          pins_changed[ii] = false;
          if (!changed_found) {
            changed_found = true;
            Serial.print(" ");
            Serial.print(ii);
          }
        } else if (changed_found) {
          Serial.print("-");
          Serial.print(ii-1);
          changed_found = false;
        }
      }
      if (changed_found) {
        Serial.print("-");
        Serial.print(-1);
        changed_found = false;
      }
      Serial.println();

      if (0 == pin_test_mode) {
        pin_test_mode = 1;
        Serial.print("PULLUP :: TEST TO GND\n  ");
      } else {
        pin_test_mode = 0;
        Serial.print("PULLDOWN :: TEST to 3.3V\n  ");
      }
      for (ii = 0; ii <= index_pinLast_used; ii++) {
        PinName pin_name = (PinName)ii;
        if (pinLast[ii] != 0xff) {
          if (0 == pin_test_mode)
            pinMode(pin_name, INPUT_PULLDOWN);
          else
            pinMode(pin_name, INPUT_PULLUP);
          delayMicroseconds(20);
          pinLast[ii] = digitalReadFast(pin_name);
          if (pin_test_mode != pinLast[ii]) {
            Serial.print("d#=");
            Serial.print(ii);
            if (ii < count_pin_names) {
              Serial.print("(");
              Serial.print(pinNameToStr(ii));
              Serial.print(")");
            }
            Serial.print(" val=");
            Serial.println(pinLast[ii]);
          }
        }
      }
      print_all_gpio_regs();
    }
  }
}
