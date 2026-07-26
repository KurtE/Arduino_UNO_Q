/*
 *    digitalWriteFast_zephyr.h - A quick and dirty digitalWriteFast
 *    and digitalToggleFast for STM32 based sezphyr boards. There are better
 *    versions out there, but this good enough for my testing
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */
#include <Arduino.h>
#include "digitalWriteFast_zephyr.h"
#include <zephyr/drivers/gpio.h>

#include "wiring_private.h"
using namespace zephyr::arduino;

static  GPIO_TypeDef * const port_table[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI
#ifndef STM32U5
    ,GPIOJ, GPIOK 
#endif    
};

static const char *pin_names[] = {
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




struct gpio_stm32_config_head {
  /* gpio_driver_config needs to be first */
  struct gpio_driver_config common;
  /* port base address */
  uint32_t *base;
};


uint8_t mapPinNameToPin(PinName pin_name) {
  uint8_t pin_on_port = pin_name & 0xf;
  GPIO_TypeDef  * const port = port_table[pin_name >> 4];
  for (uint8_t pin_num = 0; pin_num < NUM_OF_DIGITAL_PINS; pin_num++) {
      const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin_num].port->config;  
      GPIO_TypeDef *portX = (GPIO_TypeDef *)cfg->base;

      if ((portX == port) && (arduino_pins[pin_num].pin == pin_on_port)) {
          return pin_num;
      }
    }
    return 0xff;  // pin name not in Arduino Pin list
}

GPIO_TypeDef const *mapPinNameToPortAndPin(PinName pin_name, uint8_t *port_pin) {
  if (pin_name >= PX_COUNT) return nullptr;

  if (port_pin != nullptr) *port_pin = pin_name & 0xf;
  return port_table[pin_name >> 4];
}

PinName mapPinToPinName(uint8_t pin) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *port = (GPIO_TypeDef *)cfg->base;

  // now find this port in our Port list;
  for (uint8_t i = 0; i < (sizeof(port_table)/sizeof(port_table[0])); i++) {
    if (port == port_table[i]) {
      return (PinName)((i << 4) | arduino_pins[pin].pin);
    }
  }
  return (PinName)PX_INVALID;
}

void pinMode(PinName pin_name, PinMode mode) {
  // I am going to try to rely on Zephyr to do this, as to not have to replicate a lot of the zephyr code.
  // first main hack, See if I can find the GPIO port object that has points to the right GPIO object.
  pin_size_t pin_num = 0xff;
  uint8_t pin_on_port = pin_name & 0xf;
  GPIO_TypeDef  * const port = port_table[pin_name >> 4];
  uint8_t pin_match_port = 0xff;

  for (pin_num = 0; pin_num < NUM_OF_DIGITAL_PINS; pin_num++) {
      const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin_num].port->config;  
      GPIO_TypeDef *portX = (GPIO_TypeDef *)cfg->base;

      if (portX == port) {
        pin_match_port = pin_num;
        // Found an exact match so simply return it.
        if (arduino_pins[pin_num].pin == pin_on_port) {
          //Serial.print("pinMode(");
          //Serial.print(pin_name, HEX);
          //Serial.print(") mapped to pin: ");
          //Serial.println(pin_num);
          pinMode(pin_num, mode);
          return;
        }

      }

    }

  if (pin_match_port == 0xff) {
    //Serial.print("pinMode(");
    //Serial.print(pin_name, HEX);
    //Serial.println(") Failed - did not find port");
    return; 
  }
  // lets try to re

 //Serial.print("pinMode(");
 //Serial.print(pin_name, HEX);
 //Serial.print(") using port for pin: ");
 //Serial.println(pin_match_port);
  if (mode == INPUT) { // input mode
    gpio_pin_configure(arduino_pins[pin_match_port].port, pin_on_port, 
                       GPIO_INPUT | GPIO_ACTIVE_HIGH);
  } else if (mode == INPUT_PULLUP) { // input with internal pull-up
    gpio_pin_configure(arduino_pins[pin_match_port].port, pin_on_port, 
                       GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_HIGH);
  } else if (mode == INPUT_PULLDOWN) { // input with internal pull-down
    gpio_pin_configure(arduino_pins[pin_match_port].port, pin_on_port, 
                       GPIO_INPUT | GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH);
  } else if (mode == OUTPUT) { // output mode
    gpio_pin_configure(arduino_pins[pin_match_port].port, pin_on_port, 
                       GPIO_OUTPUT_LOW | GPIO_ACTIVE_HIGH);
  }

}

void digitalWriteFast(uint8_t pin, PinStatus val) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *port = (GPIO_TypeDef *)cfg->base;
  //uint16_t mask = mask_table[arduino_pins[pin].pin];
  uint16_t mask = 1 << arduino_pins[pin].pin;

  if (val) port->BSRR = mask;
  else port->BSRR = (uint32_t)(mask << 16);
}

// This version you takes in a pin name (PinName) like LED_RED
void digitalWriteFast(PinName pin_name, PinStatus val) {
  uint16_t mask = 1 << (pin_name & 0xf);
  GPIO_TypeDef  * const port = port_table[pin_name >> 4];
  if (val) port->BSRR = mask;
  else port->BSRR = (uint32_t)(mask << 16);
}



// Toggles the state of an IO pin - pin number version

void digitalToggleFast(uint8_t pin) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *portX = (GPIO_TypeDef *)cfg->base;
  uint16_t pin_mask = 1 << (arduino_pins[pin].pin);

  if (portX->ODR & pin_mask) portX->BSRR = (uint32_t)(pin_mask << 16);
  else portX->BSRR = pin_mask;
}

// Toggles the state of an IO pin - pin name version
void digitalToggleFast(PinName pin_name) {
  uint16_t pin_mask = 1 << (pin_name & 0xf);
  GPIO_TypeDef  * const portX = port_table[pin_name >> 4];

  if (portX->ODR & pin_mask) portX->BSRR = (uint32_t)(pin_mask << 16);
  else portX->BSRR = pin_mask;
}


// Reads the state of an IO pin - pin number version
PinStatus digitalReadFast(uint8_t pin) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *portX = (GPIO_TypeDef *)cfg->base;
  uint16_t pin_mask = 1 << (arduino_pins[pin].pin);

  return (portX->IDR & pin_mask) ? HIGH : LOW;
}

// Reads the state of an IO pin - pin name version
PinStatus digitalReadFast(PinName pin_name) {
  uint16_t pin_mask = 1 << (pin_name & 0xf);
  GPIO_TypeDef  * const portX = port_table[pin_name >> 4];

  return (portX->IDR & pin_mask) ? HIGH : LOW;
}


// Map PinName to String
const char *pinNameToStr(PinName pin_name) {
  return (pin_name < PX_COUNT)? pin_names[pin_name] : nullptr;
}


void pinNameSetModer(PinName pin_name, uint8_t pin_mode) {
  GPIO_TypeDef  * const port = port_table[pin_name >> 4];
  uint8_t pin = pin_name & 0xf;

  uint32_t moder = port->MODER;
  uint32_t mask = ~(0x3 << (pin * 2));
  moder = (moder & mask) | (pin_mode << (pin * 2));
  port->MODER = moder;
}

void pinSetModer(uint8_t pin, uint8_t pin_mode) {
    PinName pn = mapPinToPinName(pin);
    if (pn != PX_INVALID) {
        pinNameSetModer(pn, pin_mode);
    }
}

void PinNameSetAFR(PinName pin_name, uint8_t af) {
  GPIO_TypeDef  * const port = port_table[pin_name >> 4];
  uint8_t pin = pin_name & 0xf;

  uint32_t moder = port->MODER;
  uint32_t mask = ~(0x3 << (pin * 2));
  moder = (moder & mask) | (0x2 << (pin * 2));
  port->MODER = moder;

  if (pin < 8) {
    port->AFR[0] = ( port->AFR[0] & ~(0xf << (pin * 4)) ) | (af << (pin * 4));
  } else {
    pin -= 8;
    port->AFR[1] = ( port->AFR[1] & ~(0xf << (pin * 4)) ) | (af << (pin * 4));
  }
}

void pinSetAFR(uint8_t pin, uint8_t af) {
  PinName pn = mapPinToPinName(pin);
    if (pn != PX_INVALID) {
        PinNameSetAFR(pn, af);
    }
}  
