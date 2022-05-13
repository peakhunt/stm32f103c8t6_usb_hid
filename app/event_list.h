#ifndef __EVENT_LIST_DEF_H__
#define __EVENT_LIST_DEF_H__

#define DISPATCH_EVENT_USB_INIT             1
#define DISPATCH_EVENT_USB_IDLE             2

#define DISPATCH_EVENT_TIMER_TICK           3
#define DISPATCH_EVENT_UART_CLI_RX          4

#define DISPATCH_EVENT_USB_CLI_RX           5

#define DISPATCH_EVENT_ADC_COMPLETE         6

#define DISPATCH_EVENT_PIN_CHANGE           8

#define DISPATCH_EVENT_HEATER_STATE         10

#define DISPATCH_EVENT_GPIO_IN_0            12
#define DISPATCH_EVENT_GPIO_IN_1            13
#define DISPATCH_EVENT_GPIO_IN_2            14

#define DISPATCH_EVENT_I2C_DMA_COMPLETE     16
#define DISPATCH_EVENT_SCREEN_UPDATE_DONE   17

#endif //!__EVENT_LIST_DEF_H__
