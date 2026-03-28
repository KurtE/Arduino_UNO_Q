#include <elapsedMillis.h>

#include <ST77XX_zephyr.h>

#define USE_JSPI_PINS

#define TFT_DC 4
#define TFT_CS 2
#define TFT_RST 3


// Option 1: use any pins but a little slower
// Note: code will detect if specified pins are the hardware SPI pins
//       and will use hardware SPI if appropriate
// For 1.44" and 1.8" TFT with ST7735 use
//ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// For 1.54" or other TFT with ST7789, This has worked with some ST7789
// displays without CS pins, for those you can pass in -1 or 0xff for CS
// More notes by the tft.init call
//ST7789_t3 tft = ST7789_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
// For 1.44" and 1.8" TFT with ST7735 use
//ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);

// For 1.54" TFT with ST7789
//ST7789_t3 tft = ST7789_t3(TFT_CS, TFT_DC, TFT_RST);

// For 3.5" or 4.0" TFT with ST7796


ST7796_zephyr tft = ST7796_zephyr(&SPI, TFT_CS, TFT_DC, TFT_RST);

uint8_t test_screen_rotation = 0;

void set_pin_moder(GPIO_TypeDef *port, uint8_t pin, uint8_t pin_mode) {
  // Set the MODER = could be done in fewer steps
  uint32_t moder = port->MODER;
  uint32_t mask = ~(0x3 << (pin * 2));
  moder = (moder & mask) | (pin_mode << (pin * 2));
  port->MODER = moder;
}

void set_gpio_pin_mode(GPIO_TypeDef *port, uint8_t pin, uint8_t af) {
  // Set the MODER = could be done in fewer steps
  uint32_t moder = port->MODER;
  uint32_t mask = ~(0x3 << (pin * 2));
  moder = (moder & mask) | (0x2 << (pin * 2));
  port->MODER = moder;

  if (pin < 8) {
    port->AFR[0] = port->AFR[0] & ~(0xf << (pin * 4)) | (af << (pin * 4));
  } else {
    pin -= 8;
    port->AFR[1] = port->AFR[1] & ~(0xf << (pin * 4)) | (af << (pin * 4));
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




void setup() {
  Serial.begin(115200);
  long unsigned debug_start = millis();
  while (!Serial && ((millis() - debug_start) <= 5000))
    ;
  Serial.println("Setup");
  // Use this initializer if you're using a 1.8" TFT 128x160 displays
  //tft.initR(INITR_BLACKTAB);

  // Or use this initializer (uncomment) if you're using a 1.44" TFT (128x128)
  //tft.initR(INITR_144GREENTAB);

  // Or use this initializer (uncomment) if you're using a .96" TFT(160x80)
  //tft.initR(INITR_MINI160x80);

  // Or use this initializer (uncomment) for Some 1.44" displays use different memory offsets
  // Try it if yours is not working properly
  // May need to tweek the offsets
  //tft.setRowColStart(32,0);

  // Or use this initializer (uncomment) if you're using a 1.54" 240x240 TFT
  //tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  //tft.init(240, 320);           // Init ST7789 320x240

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  //  tft.init(240, 320);           // Init ST7789 320x240

  // OR use this initializer (uncomment) if using a 240x240 clone
  // that does not have a CS pin2.0" 320x240 TFT:
  //tft.init(240, 240, SPI_MODE2);           // Init ST7789 240x240 no CS

  // Or for ST7796 3.5 or 4"
#ifdef USE_JSPI_PINS
  // they are pins c2, c3, d1
  set_gpio_pin_mode(GPIOC, 2, 5);
  set_gpio_pin_mode(GPIOC, 3, 5);
  set_gpio_pin_mode(GPIOD, 1, 5);

  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
#endif

  tft.init(320, 480);
  tft.fillScreen(ST77XX_RED);
  show_all_gpio_regs();
}

void loop() {
  elapsedMillis em;
  tft.fillScreen(ST77XX_BLACK);
  tft.fillScreen(ST77XX_NAVY);
  tft.fillScreen(ST77XX_DARKGREEN);
  tft.fillScreen(ST77XX_DARKCYAN);
  tft.fillScreen(ST77XX_MAROON);
  tft.fillScreen(ST77XX_PURPLE);
  tft.fillScreen(ST77XX_OLIVE);
  tft.fillScreen(ST77XX_LIGHTGREY);
  tft.fillScreen(ST77XX_DARKGREY);
  tft.fillScreen(ST77XX_BLUE);
  tft.fillScreen(ST77XX_GREEN);
  tft.fillScreen(ST77XX_CYAN);
  tft.fillScreen(ST77XX_RED);
  tft.fillScreen(ST77XX_MAGENTA);
  tft.fillScreen(ST77XX_YELLOW);
  tft.fillScreen(ST77XX_WHITE);
  tft.fillScreen(ST77XX_ORANGE);
  tft.fillScreen(ST77XX_GREENYELLOW);
  tft.fillScreen(ST77XX_PINK);

  uint32_t dt = em;
#if 0
  Serial.print(dt);
  delay(250);
  em = 0;
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_BLACK);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_NAVY);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_DARKGREEN);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_DARKCYAN);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_MAROON);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_PURPLE);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_OLIVE);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_LIGHTGREY);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_DARKGREY);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_BLUE);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_GREEN);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_CYAN);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_RED);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_MAGENTA);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_YELLOW);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_WHITE);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_ORANGE);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_GREENYELLOW);
  tft.fillRectCB(0, 0, tft.width(), tft.height(), ST77XX_PINK);
  dt = em;
  Serial.print(" ");
#endif
  Serial.println((uint32_t)em);
}
