#include "../device.h"

#include <stdio.h>
#include <string.h>

static struct device device_usb_soundcard;

static const uint16_t PRODUCT_ID = 0x0529;

static int usb_soundcard_send_sidetone(hid_device* device_handle, uint8_t num);

void razer_usb_soundcard_init(struct device** device)
{
    device_usb_soundcard.idVendor               = VENDOR_RAZER;
    device_usb_soundcard.idProductsSupported    = &PRODUCT_ID;
    device_usb_soundcard.numIdProducts          = 1;

    strncpy(device_usb_soundcard.device_name, "Razer USB Sound Card", sizeof(device_usb_soundcard.device_name));

    device_usb_soundcard.capabilities   = B(CAP_SIDETONE);
    device_usb_soundcard.send_sidetone  = &usb_soundcard_send_sidetone;

    *device = &device_usb_soundcard;
}

static int usb_soundcard_send_sidetone(hid_device* device_handle, uint8_t num)
{
    int ret = -1;

    const unsigned char data_on_pre[10] = {
        0x21, 0x01, 0x00, 0x02, 0x00, 0x0b, 0x02, 0x00, 0x00, 0xEA
    };
    const unsigned char data_on[9] = {
        0x21, 0x01, 0x00, 0x01, 0x00, 0x0b, 0x01, 0x00, 0x00
    };

    const unsigned char data_off_pre[10] = {
        0x21, 0x01, 0x00, 0x02, 0x00, 0x0b, 0x02, 0x00, 0x00, 0xEA
    };
    const unsigned char data_off[9] = {
        0x21, 0x01, 0x00, 0x01, 0x00, 0x0b, 0x01, 0x00, 0x01
    };

    if (num) {
        ret = hid_write(device_handle, data_on_pre, sizeof(data_on_pre));
        printf("[*][RZR](ON) Response to pre: %d\n", ret);

        ret = hid_write(device_handle, data_on, sizeof(data_on));
        printf("[*][RZR](ON) Response to cmd: %d\n", ret);
    } else {
        ret = hid_write(device_handle, data_off_pre, sizeof(data_off_pre));
        printf("[*][RZR](OFF) Response to pre: %d\n", ret);

        ret = hid_write(device_handle, data_off, sizeof(data_off));
        printf("[*][RZR](OFF) Response to cmd: %d\n", ret);
    }

    return ret;
}