#pragma once
#include <Arduino.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>

class memorySerial : public arduino::Stream {
	enum {BUFFER_SIZE=1024};

  // override functions from Stream
  public:
    memorySerial() {
      ring_buf_init(&_ringbuf, sizeof(_buffer), _buffer);
    }

	void begin(__attribute__((unused)) unsigned long baudRate=115200);

	void begin(__attribute__((unused)) unsigned long baudrate,
			   __attribute__((unused)) uint16_t config) {
      begin(baudrate);
	}

	void end() {
	}

    virtual int available() override;
    virtual int read() override;
    virtual int peek() override;
	int read(uint8_t *buffer, size_t size); // currently not like Arduino only what is there...
    size_t read (char);
  
    //override functions from Print
    virtual size_t write(uint8_t) override;
    virtual size_t write(const uint8_t *buffer, size_t size) override;
    virtual int availableForWrite() override;
  
  protected:
	uint8_t _buffer[BUFFER_SIZE];
	struct ring_buf _ringbuf;
};