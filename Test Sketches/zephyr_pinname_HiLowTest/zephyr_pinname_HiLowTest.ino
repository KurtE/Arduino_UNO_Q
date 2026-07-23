#include <digitalWriteFast_zephyr.h>

const char *pin_names[] = {
  // clang-format off
    "PA_0", "PA_1", "PA_2", "PA_3","PA_4", "PA_5", "PA_6", "PA_7", 
    "PA_8", "PA_9", "PA_10", "PA_11", "PA_12", "PA_13", "PA_14", "PA_15", 
    "PB_0", "PB_1", "PB_2", "PB_3", "PB_4", "PB_5", "PB_6", "PB_7", 
    "PB_8", "PB_9", "PB_10", "PB_11", "PB_12", "PB_13", "PB_14", "PB_15",
    "PC_0", "PC_1", "PC_2", "PC_3", "PC_4","PC_5", "PC_6", "PC_7", 
    "PC_8", "PC_9", "PC_10", "PC_11", "PC_12", "PC_13", "PC_14", "PC_15",
    "PD_0", "PD_1", "PD_2", "PD_3", "PD_4", "PD_5", "PD_6", "PD_7", 
    "PD_8", "PD_9", "PD_10", "PD_11", "PD_12", "PD_13", "PD_14", "PD_15", 
    "PE_0", "PE_1", "PE_2", "PE_3", "PE_4", "PE_5", "PE_6", "PE_7", 
    "PE_8", "PE_9", "PE_10", "PE_11", "PE_12", "PE_13", "PE_14", "PE_15", 
    "PF_0", "PF_1", "PF_2", "PF_3", "PF_4", "PF_5", "PF_6", "PF_7", 
    "PF_8", "PF_9", "PF_10", "PF_11","PF_12", "PF_13", "PF_14", "PF_15", 
    "PG_0", "PG_1", "PG_2", "PG_3", "PG_4", "PG_5", "PG_6","PG_7", 
    "PG_8", "PG_9", "PG_10", "PG_11", "PG_12", "PG_13", "PG_14", "PG_15", 
    "PH_0", "PH_1", "PH_2", "PH_3", "PH_4", "PH_5", "PH_6", "PH_7", 
    "PH_8", "PH_9", "PH_10", "PH_11","PH_12", "PH_13", "PH_14", "PH_15", 
    "PI_0", "PI_1", "PI_2", "PI_3", "PI_4", "PI_5", "PI_6","PI_7", 
    "PI_8", "PI_9", "PI_10", "PI_11", "PI_12", "PI_13", "PI_14", "PI_15", 
    "PJ_0", "PJ_1", "PJ_2", "PJ_3", "PJ_4", "PJ_5", "PJ_6", "PJ_7", 
    "PJ_8", "PJ_9", "PJ_10", "PJ_11", "PJ_12","PJ_13", "PJ_14", "PJ_15", 
    "PK_0", "PK_1", "PK_2", "PK_3", "PK_4", "PK_5", "PK_6", "PK_7",

  // clang-format on
};

const uint8_t count_pin_names = sizeof(pin_names) / sizeof(pin_names[1]);
uint8_t pin_test_mode = 1;

// This one is setup currently for Portenta H7
uint8_t pinLast[count_pin_names] = {
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

#if defined(ARDUINO_UNO_Q)
static constuint16_t pinname_exclude_list[11] = {
  // LEDS PH_10=15, LED_MATRIX=PF0-10, SPI RDR=PG_13, Analog switch=PA_2, BOOT0 PH_3
  0b0000000000000100, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000,  // GPIOA-D
  0b0000000000000000, 0b0000011111111111, 0b0010000000000000, 0b1111110000001000,  // GPIOE-PH
  0b0000000000000000, 0b0000000000000000, 0b0000000000000000,                      // GPIOI-K
};

#elif defined(ARDUINO_PORTENTA_H7_M7)
static const uint16_t pinname_exclude_list[11] = {
  // LEDS PH_10=15, LED_MATRIX=PF0-10, SPI RDR=PG_13, Analog switch=PA_2, BOOT0 PH_3
  0b1000000010101110, 0b0011110000100011, 0b0101111100110011, 0b1111111100000111, // GPIOA-D
  0b1111111111110011, 0b1111111111111111, 0b1011100101110111, 0b0000000000111111, // GPIOE-PH
  0b0001100000000000, 0b1111000000111111, 0b11111101                              // GPIOI-K
};
#elif defined(ARDUINO_GIGA)
static const uint16_t pinname_exclude_list[11] = {0};
#endif

#ifndef NUM_DIGITAL_PINS
#define NUM_DIGITAL_PINS (22u)
//#if DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios) > 0
//uint16_t PINS_COUNT = DT_PROP_LEN(DT_PATH(zephyr_user), digital_pin_gpios);
const uint8_t PINS_COUNT = sizeof(pinLast);
//#endif
#endif

int index_pinLast_used = -1;

bool pins_changed[count_pin_names];

extern void allPinTest();

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 10000)
    ;

  // default to only Arduino Pins.
  //enable_only_arduino_pins();
  exclude_pins();
  // find the last pin name used
  for (index_pinLast_used = sizeof(pinLast) - 1; index_pinLast_used >= 0; index_pinLast_used--) {
    if (pinLast[index_pinLast_used] == 0) break;
  }

  

  Serial.print("NUM_DIGITAL_PINS: ");
  Serial.println(NUM_DIGITAL_PINS, DEC);
  Serial.println("Pins included in test:");
  int index_first_in_series = -1;
  for (int i = 0; i <= index_pinLast_used; i++) {
    if (pinLast[i] != 0xff) {
      if (index_first_in_series == -1) index_first_in_series = i;
    } else {
      // end of series
      if (index_first_in_series != -1) {
        Serial.print(" ");
        Serial.print(pin_names[index_first_in_series]);
        if (index_first_in_series != (i - 1)) {
          Serial.print("-");
          Serial.print(pin_names[i - 1]);
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
  for (uint8_t i = 0; i < 11; i++) {
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
        Serial.print(pin_names[ii]);
        Serial.print(") ");
        Serial.flush();
      }
      pinMode(pin_name, INPUT_PULLUP);
      delayMicroseconds(5);
      pinLast[ii] = digitalReadFast(pin_name);
      if (!pinLast[ii]) {
        Serial.print("\nd#=");
        Serial.print(pin_names[ii]);
        Serial.print(" val=");
      }
      Serial.print(pinLast[ii]);
      Serial.print(',');
    }
  }
  Serial.println();
  Serial.println();
  show_all_gpio_regs();
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
          Serial.print(pin_names[ii]);
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
              Serial.print(pin_names[ii]);
              Serial.print(")");
            }
            Serial.print(" val=");
            Serial.println(pinLast[ii]);
          }
        }
      }
      show_all_gpio_regs();
    }
  }
}
void print_gpio_regs(const char *name, GPIO_TypeDef *port) {
  //printk("GPIO%s(%p) %08X %08X %08x\n", name, port, port->MODER, port->AFR[0], port->AFR[1]);
  Serial.print("GPIO");
  Serial.print(name);
  Serial.print(" ");
  uint32_t moder = port->MODER;
  Serial.print(moder, HEX);
  Serial.print(" : ");
  for (uint8_t i = 0; i < 16; i++) {
    switch (moder & 0xC0000000) {
      case 0x00000000ul: Serial.print("I"); break;
      case 0x40000000ul: Serial.print("O"); break;
      case 0x80000000ul: Serial.print("F"); break;
      default: Serial.print("A"); break;
    }
    moder <<= 2;
  }
  Serial.print(" ");
  Serial.print(port->AFR[0], HEX);
  Serial.print(" ");
  Serial.print(port->AFR[1], HEX);
  Serial.print(" ");
  Serial.print(port->IDR, HEX);
  Serial.print(" ");
  Serial.print(port->ODR, HEX);
  Serial.print(" ");
  uint32_t pupdr = port->PUPDR;
  Serial.print(pupdr, HEX);
  Serial.print(" : ");
  for (uint8_t i = 0; i < 16; i++) {
    switch (pupdr & 0xC0000000) {
      case 0x00000000ul: Serial.print("-"); break;
      case 0x40000000ul: Serial.print("U"); break;
      case 0x80000000ul: Serial.print("D"); break;
      default: Serial.print("?"); break;
    }
    pupdr <<= 2;
  }
  Serial.println();
}

void show_all_gpio_regs() {
  print_gpio_regs("A", (GPIO_TypeDef *)GPIOA_BASE);
  print_gpio_regs("B", (GPIO_TypeDef *)GPIOB_BASE);
  print_gpio_regs("C", (GPIO_TypeDef *)GPIOC_BASE);
  print_gpio_regs("D", (GPIO_TypeDef *)GPIOD_BASE);
  print_gpio_regs("E", (GPIO_TypeDef *)GPIOE_BASE);
  print_gpio_regs("F", (GPIO_TypeDef *)GPIOF_BASE);
  print_gpio_regs("G", (GPIO_TypeDef *)GPIOG_BASE);
  print_gpio_regs("H", (GPIO_TypeDef *)GPIOH_BASE);
  print_gpio_regs("I", (GPIO_TypeDef *)GPIOI_BASE);
}

