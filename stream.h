#ifndef __STREAM_H__
#define __STREAM_H__

#include <stddef.h>
#include <stdint.h>
#include "FdSerial.h"

class Stream {
public:
    Stream();
    ~Stream();
    bool begin(int baudRate);
    bool isOpen() { return m_isOpen; }
    int available();
    int read();
    size_t write(uint8_t);
    size_t write(const char *buffer, size_t size);
    void print(const char *buf);
    void print(int value, int base = 10);
    void println(const char *buf);
    void println(int value, int base = 10);
    void println();
private:
    FdSerial_t m_port;
    bool m_isOpen;
    int m_ch;
};

extern Stream Serial;

// these belong somewhere else!
uint32_t millis();
void delay(int ms);

#endif
