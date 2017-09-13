#ifndef __USB_GPIO_REPORT_QUEUE_H__
#define __USB_GPIO_REPORT_QUEUE_H__

#include <stdint.h>

extern void usb_gpio_report_queue_do_work(void);
extern void handle_gpio_irq(void);

#endif //!__USB_GPIO_REPORT_QUEUE_H__
