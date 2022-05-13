#include "stm32f1xx_hal.h"
#include "usbd_customhid.h"
#include "gpio.h"

#include "soft_timer.h"
#include "mainloop_timer.h"
#include "event_dispatcher.h"
#include "event_list.h"

#include "hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

//
// 1 byte report ID
// 1 byte for 8 buttons
// 2 byte for X/Y
// total 4 bytes
//
static uint8_t  _hid_report[16];

static void
fill_report1(uint8_t* buf)
{
  // simple brute force approach
  //
  buf[0] = 1;   // report ID 1

  buf[1] = 0;       // data

  // joystick button 1
  if(HAL_GPIO_ReadPin(joystick_button0_GPIO_Port, joystick_button0_Pin) == GPIO_PIN_SET)
  {
    buf[1] |= (1 << 0);
  }

  // joystick button 2
  if(HAL_GPIO_ReadPin(joystick_button1_GPIO_Port, joystick_button1_Pin) == GPIO_PIN_SET)
  {
    buf[1] |= (1 << 1);
  }

  // joystick button 3
  if(HAL_GPIO_ReadPin(joystick_button2_GPIO_Port, joystick_button2_Pin) == GPIO_PIN_SET)
  {
    buf[1] |= (1 << 2);
  }

  // joystick button 4
  if(HAL_GPIO_ReadPin(joystick_button3_GPIO_Port, joystick_button3_Pin) == GPIO_PIN_SET)
  {
    buf[1] |= (1 << 3);
  }

  // joystick button 5
  // joystick button 6
  // joystick button 7
  // joystick button 8

  // joystick X
  buf[2] = 128;
  if(HAL_GPIO_ReadPin(joystick_left_GPIO_Port, joystick_left_Pin) == GPIO_PIN_SET)
  {
    buf[2] = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_right_GPIO_Port, joystick_right_Pin) == GPIO_PIN_SET)
  {
    buf[2] = 255;
  }

  //
  // joystick Y
  //
  buf[3] = 128;
  if(HAL_GPIO_ReadPin(joystick_up_GPIO_Port, joystick_up_Pin) == GPIO_PIN_SET)
  {
    buf[3] = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_down_GPIO_Port, joystick_down_Pin) == GPIO_PIN_SET)
  {
    buf[3] = 255;
  }
}

static void
fill_report2(uint8_t* buf)
{
  // simple brute force approach
  //
  buf[0] = 2;   // report ID 2

  buf[1] = 0;       // data

  // joystick button 1
  buf[1] |= (1 << 0);

  // joystick button 2
  buf[1] |= (1 << 1);

  // joystick button 3
  buf[1] |= (1 << 2);

  // joystick button 4
  buf[1] |= (1 << 3);

  // joystick button 5
  // joystick button 6
  // joystick button 7
  // joystick button 8

  // joystick X
  buf[2] = 128;

  // joystick Y
  buf[3] = 128;
}

static void
send_report(void)
{
  fill_report1(&_hid_report[0]);
  fill_report2(&_hid_report[4]);

  if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, _hid_report, 8) == USBD_BUSY)
  {
    // XXX debug point
    // we are not expecting to receive any busy return with this mechanism
    while(1)
      ;
  }
  else
  {
  }
}

static void
usb_idle_event(uint32_t event)
{
  send_report();
}

static void
usb_init_event(uint32_t event)
{
  send_report();
}

void
hid_init(void)
{
  event_register_handler(usb_idle_event, DISPATCH_EVENT_USB_IDLE);
  event_register_handler(usb_init_event, DISPATCH_EVENT_USB_INIT);
}
