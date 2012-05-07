/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @brief USB virtual serial terminal
 */

#include <string.h>

#include "wirish.h"
#include "usb_cdcacm.h"
#include "usb.h"

#define USB_TIMEOUT 50

USBSerial::USBSerial(void) {
}

void USBSerial::begin(void) {
    usb_cdcacm_enable(BOARD_USB_DISC_DEV, BOARD_USB_DISC_BIT);
}

void USBSerial::end(void) {
    usb_cdcacm_disable(BOARD_USB_DISC_DEV, BOARD_USB_DISC_BIT);
}

void USBSerial::write(uint8 ch) {
    const uint8 buf[] = {ch};
    this->write(buf, 1);
}

void USBSerial::write(const char *str) {
    this->write(str, strlen(str));
}

void USBSerial::write(const void *buf, uint32 len) {
    if (!this->isConnected() || !buf) {
        return;
    }

    uint32 txed = 0;
    uint32 old_txed = 0;
    uint32 start = millis();

    while (txed < len && (millis() - start < USB_TIMEOUT)) {
        txed += usb_cdcacm_tx((const uint8*)buf + txed, len - txed);
        if (old_txed != txed) {
            start = millis();
        }
        old_txed = txed;
    }
}

uint32 USBSerial::available(void) {
    return usb_cdcacm_data_available();
}

uint32 USBSerial::read(void *buf, uint32 len) {
    if (!buf) {
        return 0;
    }

    uint32 rxed = 0;
    while (rxed < len) {
        rxed += usb_cdcacm_rx((uint8*)buf + rxed, len - rxed);
    }

    return rxed;
}

/* Blocks forever until 1 byte is received */
uint8 USBSerial::read(void) {
    uint8 b;
    this->read(&b, 1);
    return b;
}

uint8 USBSerial::pending(void) {
    return usb_cdcacm_get_pending();
}

uint8 USBSerial::isConnected(void) {
    return usb_is_connected(USBLIB) && usb_is_configured(USBLIB);
}

uint8 USBSerial::getDTR(void) {
    return usb_cdcacm_get_dtr();
}

uint8 USBSerial::getRTS(void) {
    return usb_cdcacm_get_rts();
}

USBSerial SerialUSB;
