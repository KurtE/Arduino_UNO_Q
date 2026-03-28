#include <Arduino_RouterBridge.h>
#define SerialX Serial2
#define LED_GREEN 51

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
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("Start SerialX test");
  Monitor.begin(115200);
  SerialX.begin(115200);
  show_all_gpio_regs();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  int ich;
  int cnt = Monitor.available();
  if (cnt) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    for (;cnt; cnt--) {
      SerialX.write(Monitor.read());
    } 
  }

  cnt = SerialX.available();
  if (cnt) {
    digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
    for (;cnt; cnt--) {
      Monitor.write(SerialX.read());
    } 
  }

  while ((ich = SerialX.read())!= -1) Monitor.write(ich);
}
