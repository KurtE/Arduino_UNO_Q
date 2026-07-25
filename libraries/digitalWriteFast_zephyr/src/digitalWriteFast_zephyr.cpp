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
#ifndef ARDUINO_UNO_Q
    ,GPIOJ, GPIOK 
#endif    
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

PinName mapPinToPinName(uint8_t pin) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *port = (GPIO_TypeDef *)cfg->base;

  // now find this port in our Port list;
  for (uint8_t i = 0; i < (sizeof(port_table)/sizeof(port_table[0])); i++) {
    if (port == port_table[i]) {
      return (PinName)((i << 4) | arduino_pins[pin].pin);
    }
  }
  return (PinName)0xff;
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
uint16_t digitalReadFast(uint8_t pin) {
  const struct gpio_stm32_config_head *cfg = (gpio_stm32_config_head*)arduino_pins[pin].port->config;  
  GPIO_TypeDef *portX = (GPIO_TypeDef *)cfg->base;
  uint16_t pin_mask = 1 << (arduino_pins[pin].pin);

  return (portX->IDR & pin_mask) ? 1 : 0;
}

// Reads the state of an IO pin - pin name version
uint16_t digitalReadFast(PinName pin_name) {
  uint16_t pin_mask = 1 << (pin_name & 0xf);
  GPIO_TypeDef  * const portX = port_table[pin_name >> 4];

  return (portX->IDR & pin_mask) ? 1 : 0;
}

