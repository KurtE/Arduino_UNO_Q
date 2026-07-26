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
#ifndef _ZEPHYR_DIGITALWRITEFAST_H_
#define _ZEPHYR_DIGITALWRITEFAST_H_
#include <Arduino.h>
#if !defined(ARDUINO_ARCH_ZEPHYR)

#error "Only works on Zephyr (STM32)"
#endif



typedef enum {
    PA_0=0x00, PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15,
    PB_0=0x10, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,
    PC_0=0x20, PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_13, PC_14, PC_15,
    PD_0=0x30, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_14, PD_15,
    PE_0=0x40, PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14, PE_15,
    PF_0=0x50, PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7, PF_8, PF_9, PF_10, PF_11, PF_12, PF_13, PF_14, PF_15,
    PG_0=0x60, PG_1, PG_2, PG_3, PG_4, PG_5, PG_6, PG_7, PG_8, PG_9, PG_10, PG_11, PG_12, PG_13, PG_14, PG_15,
    PH_0=0x70, PH_1, PH_2, PH_3, PH_4, PH_5, PH_6, PH_7, PH_8, PH_9, PH_10, PH_11, PH_12, PH_13, PH_14, PH_15,
    PI_0=0x80, PI_1, PI_2, PI_3, PI_4, PI_5, PI_6, PI_7, 
#if !defined(STM32U5)  // UNO_Q does not have the vollowing pins
    PI_8, PI_9, PI_10, PI_11, PI_12, PI_13, PI_14, PI_15,
    PJ_0=0x90, PJ_1, PJ_2, PJ_3, PJ_4, PJ_5, PJ_6, PJ_7, PJ_8, PJ_9, PJ_10, PJ_11, PJ_12, PJ_13, PJ_14, PJ_15,
    PK_0=0xA0, PK_1, PK_2, PK_3, PK_4, PK_5, PK_6, PK_7,
#endif
    PX_COUNT
} PinName;



extern void pinMode(PinName pin_name, PinMode mode);

extern uint8_t mapPinNameToPin(PinName pin_name);
extern PinName mapPinToPinName(uint8_t pin);

// quick and dirty digitalWriteFast

// Sets the state of an IO pin
// Two versions - this version you takes in a pin number as uint8_t type - uint8_t
extern void digitalWriteFast(uint8_t pin, PinStatus val);
extern void digitalWriteFast(PinName pin_name, PinStatus val);

inline void digitalWriteFast(uint8_t pin, int status) {
    digitalWriteFast(pin, (PinStatus)status);
};

inline void digitalWriteFast(PinName pin_name, int status) {
    digitalWriteFast(pin_name, (PinStatus)status);
};



// Toggles the state of an IO pin - pin number version

extern void digitalToggleFast(uint8_t pin);
extern void digitalToggleFast(PinName pin_name);

// Reads the state of an IO pin - pin number version
extern PinStatus digitalReadFast(uint8_t pin);
extern PinStatus digitalReadFast(PinName pin_name);

// Map PinName to String
extern const char *pinNameToStr(PinName pin_name);
inline const char *pinNameToStr(int pin_name) {
    return pinNameToStr((PinName)pin_name);
}


// Debug printing functions
extern void print_gpio_regs(const char *name, GPIO_TypeDef *port);
extern void print_all_gpio_regs();



#endif