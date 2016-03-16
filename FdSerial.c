/**
 * @file FdSerial.c
 * Full Duplex Serial adapter module.
 *
 * Copyright (c) 2008, Steve Denson
 * See end of file for terms of use.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cog.h>
#include <sys/driver.h>
#include <propeller.h>
#include <compiler.h>

#include "FdSerial.h"

static FdSerial_t *coglist;

/**
 * start initializes and starts native assembly driver in a cog.
 * @param rxpin is pin number for receive input
 * @param txpin is pin number for transmit output
 * @param mode is interface mode. see header FDSERIAL_MODE_...
 * @param baudrate is frequency of bits ... 115200, 57600, etc...
 * @returns non-zero on success
 */
int _FdSerial_start(FdSerial_t *data, int rxpin, int txpin, int mode, int baudrate)
{
    use_cog_driver(FullDuplexSerial2);

    memset(data, 0, sizeof(FdSerial_t));
    data->rx_pin  = rxpin;                  // receive pin
    data->tx_pin  = txpin;                  // transmit pin
    data->mode    = mode;                   // interface mode
    data->ticks   = _clkfreq / baudrate;    // baud
    data->rxbmask = FDSERIAL_RXBMASK;       // receive buffer mask
    data->txbmask = FDSERIAL_TXBMASK;       // transmit buffer mask
    data->buffptr = (int)&data->rxbuff[0];
    data->cogId = load_cog_driver(FullDuplexSerial2, data) + 1;

    return data->cogId;
}

/**
 * stop stops the cog running the native assembly driver 
 */
void _FdSerial_stop(FdSerial_t *data)
{
    if(data->cogId > 0) {
        cogstop(data->cogId - 1);
        data->cogId = 0;
    }
}

/**
 * rxflush empties the receive queue 
 */
void _FdSerial_rxflush(FdSerial_t *data)
{
    // clear out queue by receiving all available 
    while(_FdSerial_rxcheck(data) >= 0)
      ;
}

/**
 * Gets a byte from the receive queue if available
 * Function does not block. We move rxtail after getting char.
 * @returns receive byte 0 to 0xff or -1 if none available 
 */
int _FdSerial_rxcheck(FdSerial_t *data)
{
    int rc = -1;
    if(data->rx_tail != data->rx_head) {
        rc = data->rxbuff[data->rx_tail];
        data->rx_tail = (data->rx_tail + 1) & data->rxbmask;
    }
    return rc;
}

/**
 * Wait for a byte from the receive queue. blocks until something is ready
 * @returns received byte 
 */
int _FdSerial_rx(FdSerial_t *data)
{
  int rc;
  while ((rc = _FdSerial_rxcheck(data)) < 0)
    ;
  return rc;
}

/**
 * putbyte sends a byte on the transmit queue.
 * @param txbyte is byte to send. 
 */
int _FdSerial_tx(FdSerial_t *data, int txbyte)
{
    volatile char* txbuff = data->txbuff;
    while(data->tx_tail == ((data->tx_head + 1) & data->txbmask)) // wait for space in queue
        ;
    txbuff[data->tx_head] = txbyte;
    data->tx_head = (data->tx_head + 1) & data->txbmask;
    return txbyte;
}

/**
 * drain waits for all bytes to be sent
 */
void _FdSerial_drain(FdSerial_t *data)
{
  unsigned int waitcycles;
  while(data->tx_tail != data->tx_head)
    ;

  // wait for character to be transmitted
  // strictly speaking we only need to wait 10*data->ticks,
  // but give ourselves a bit of margin here
  waitcycles = CNT + (data->ticks << 4);
  waitcnt(waitcycles);
}

/*
+------------------------------------------------------------------------------------------------------------------------------+
�                                                   TERMS OF USE: MIT License                                                  �                                                            
+------------------------------------------------------------------------------------------------------------------------------�
�Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation    � 
�files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,    �
�modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software�
�is furnished to do so, subject to the following conditions:                                                                   �
�                                                                                                                              �
�The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.�
�                                                                                                                              �
�THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE          �
�WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR         �
�COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   �
�ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                         �
+------------------------------------------------------------------------------------------------------------------------------+
*/
