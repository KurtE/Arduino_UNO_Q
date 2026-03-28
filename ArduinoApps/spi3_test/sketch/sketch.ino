#include "SPIPeripheral.h"
#include "Arduino_RouterBridge.h"

SPIPeripheralClass<512> spi;

void setup() {
  Monitor.begin();
  delay(2000);
  Monitor.println("Begin SPI3 Test....");
  spi.begin();
}

int i = 0;
void loop() {
  spi.populate((uint8_t*)&i, 4);
  spi.ready();
  i++;
}