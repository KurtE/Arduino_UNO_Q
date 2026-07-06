#include "memorySerial.h"

void memorySerial::begin(__attribute__((unused)) unsigned long baudRate) {
  // reset the ringbuffer  
  ring_buf_reset(&_ringbuf);  
}

size_t memorySerial::write(uint8_t ch) {
  return write(&ch, 1);  
}

// 
size_t memorySerial::write(const uint8_t *buffer, size_t size) {

    // BUGBUG Should we wait for space?  currently not...
    // As I don't want the code to hangup in callback.
    auto ret = ring_buf_put(&_ringbuf, buffer, size);
    return (ret < 0)? 0 : ret;
}

int memorySerial::availableForWrite() {
	return ring_buf_space_get(&_ringbuf);  
}

int memorySerial::available() {
	return ring_buf_size_get(&_ringbuf);
  
}
int memorySerial::read() {
    uint8_t ch;
    uint32_t cb_ret = ring_buf_get(&_ringbuf, &ch, 1);
	return cb_ret ? ch : -1;  
}

int memorySerial::read(uint8_t *buffer, size_t size) { // currently not like Arduino only what is there...
    uint32_t cb_ret = ring_buf_get(&_ringbuf, buffer, size);
	return cb_ret;  
}

int memorySerial::peek() {
	uint8_t data;
	uint32_t cb_ret = ring_buf_peek(&_ringbuf, &data, 1);

	return cb_ret? data : -1;
}
