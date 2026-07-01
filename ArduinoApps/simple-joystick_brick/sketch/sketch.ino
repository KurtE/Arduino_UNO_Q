#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "Arduino_RouterBridge.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/drivers/spi.h>


// Joystick event format:
typedef struct js_event {
     uint32_t time;     // event timestamp in milliseconds
     int16_t  value;    // value
     uint8_t  type;     // event type
     uint8_t  number;   // axis/button number
 } js_event_t;

// Event type bit flags
#define JS_EVENT_BUTTON  0x01
#define JS_EVENT_AXIS    0x02
#define JS_EVENT_INIT    0x80

#define SPI_PERIPHERAL_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_spi_slave)

#define SPI_MAX_MESSAGE 8

uint32_t last_led_change_time_ms = 0;

const struct device *const spi_peripheral = DEVICE_DT_GET(DT_BUS(SPI_PERIPHERAL_NODE));
struct spi_config spi_cfg;

uint8_t rxmsg[SPI_MAX_MESSAGE] __attribute__((aligned(8)));
struct spi_buf rx;
struct spi_buf_set rx_bufs;

uint8_t txmsg[SPI_MAX_MESSAGE] __attribute__((aligned(8))) = { 0 };
struct spi_buf tx;
struct spi_buf_set tx_bufs;

volatile bool spi_data_available = false;
int spi_callback_ret = -1;

#if 0
void spi_callback(const struct device *dev, int result, void *data) {
  spi_data_available = true;
  spi_callback_ret = result;
}
#endif

#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
struct k_thread thread1_data;
k_tid_t thread1_tid;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Bridge.begin();
  Serial.begin(115200);
  //  while (!Serial && millis() < 5000) {}
  delay(5000);
  Serial.println("\n*** Joystick test program starting ***");
  Serial.flush();

  // initialize the SPI
  spi_cfg.frequency = 5000000;
  spi_cfg.operation = SPI_WORD_SET(8) | SPI_OP_MODE_SLAVE;
  rx.buf = rxmsg;
  rx.len = SPI_MAX_MESSAGE;
  rx_bufs.buffers = &rx;
  rx_bufs.count = 1;
  tx.buf = txmsg;
  tx.len = SPI_MAX_MESSAGE;
  tx_bufs.buffers = &tx;
  tx_bufs.count = 1;
  Serial.print("SPI: ");
  Serial.println((uint32_t)spi_peripheral, HEX);
  Serial.flush();
  int ret = device_init(spi_peripheral);
  Serial.println("After device_init");


  if (ret < 0) {
    delay(2000);
    Serial.print("SPI Peripheral init failed: ");
    Serial.println(ret);
  }

  Serial.println("Before spi_transceive_cb");
  Serial.flush();

  //  spi_transceive_cb(spi_peripheral, &spi_cfg, &tx_bufs, &rx_bufs, &spi_callback, nullptr);
  thread1_tid = k_thread_create(&thread1_data, thread1_stack, K_THREAD_STACK_SIZEOF(thread1_stack),
    spidev_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

}


void loop() {
  if ((millis() - last_led_change_time_ms) > 500) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    last_led_change_time_ms = millis();
  }
  delay(25);
}

void spidev_thread(void *p1, void *p2, void *p3)
{
    while (1) {

      if (spi_transceive(spi_peripheral, &spi_cfg, &tx_bufs, &rx_bufs) >= 0) {
        js_event_t jevent;
        jevent.time = *((uint32_t *)rxmsg);
        jevent.value = *((int16_t *)(&rxmsg[4]));
        jevent.type = rxmsg[6];
        jevent.number = rxmsg[7];

        switch (jevent.type) {
          case JS_EVENT_BUTTON:
            Serial.print("T: ");
            Serial.print(jevent.time);
            Serial.print(" Button: ");
            Serial.print(jevent.number);
            Serial.println(jevent.value? " Pressed" : " Released");
            break;
          case JS_EVENT_AXIS:
            Serial.print("T: ");
            Serial.print(jevent.time);
            Serial.print(" Axis: ");
            Serial.print(jevent.number);
            Serial.print(" value: ");
            Serial.println(jevent.value);
            break;
        }
    
        //spi_transceive_cb(spi_peripheral, &spi_cfg, &tx_bufs, &rx_bufs, &spi_callback, nullptr);
      }
      k_sleep(K_MSEC(1));
    }
}
