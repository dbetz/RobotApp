#include <stdio.h>
#include <propeller.h>
#include "stream.h"

Stream Serial;

Stream::Stream()
  : m_isOpen(false), m_ch(-1)
{
}

Stream::~Stream()
{
    if (m_isOpen)
        _FdSerial_stop(&m_port);
}

bool Stream::begin(int baudRate, int rxPin, int txPin)
{
    m_isOpen = _FdSerial_start(&m_port, rxPin, txPin, 0, baudRate) != 0;
    return m_isOpen;
}

int Stream::available()
{
    if (!m_isOpen)
        return -1;
        
    if (m_ch != -1)
        return 1;
        
    m_ch = _FdSerial_rxcheck(&m_port);
    
    return m_ch == -1 ? 0 : 1;
}

int Stream::read()
{
    if (!m_isOpen)
        return -1;
        
    int ch;
    if ((ch = m_ch) != -1)
        m_ch = -1;
    else
        ch = _FdSerial_rx(&m_port);
        
    return ch;
}

size_t Stream::write(uint8_t ch)
{ 
    if (!m_isOpen)
        return -1;
        
    _FdSerial_tx(&m_port, ch);
    
    return 1;
}

size_t Stream::write(const char *buffer, size_t size)
{
    if (!m_isOpen)
        return -1;
        
    int cnt = size;
    while (--cnt >= 0)
        write(*buffer++);
        
    return size;
}

void Stream::print(const char *buf)
{
    write(buf, strlen(buf));
}

void Stream::print(int value, int base)
{
    char buf[20];
    sprintf(buf, "%d", value);
    print(buf);
}
    
void Stream::println(const char *buf)
{
    write(buf, strlen(buf));
    println();
}

void Stream::println(int value, int base)
{
    print(value, base);
    println();
}
    
void Stream::println()
{
    write('\r');
    write('\n');
}

uint32_t millis()
{
    return CNT / (CLKFREQ / 1000);
}

void delay(int ms)
{
    int ticks = (CLKFREQ / 1000) * ms;
    waitcnt(CNT + ticks);
}



