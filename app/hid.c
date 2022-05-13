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
// 2 byte for X/Y
// 1 byte report ID
// 1 byte for 8 buttons
//
static uint8_t  _hid_report[5];

static void
fill_report_buf(void)
{
  // simple brute force approach
  //
  _hid_report[0] = 1;   // report ID 1

  // joystick X
  _hid_report[1] = 128;
  if(HAL_GPIO_ReadPin(joystick_left_GPIO_Port, joystick_left_Pin) == GPIO_PIN_SET)
  {
    _hid_report[1] = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_right_GPIO_Port, joystick_right_Pin) == GPIO_PIN_SET)
  {
    _hid_report[1] = 255;
  }

  //
  // joystick Y
  //
  _hid_report[2] = 128;
  if(HAL_GPIO_ReadPin(joystick_up_GPIO_Port, joystick_up_Pin) == GPIO_PIN_SET)
  {
    _hid_report[2] = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_down_GPIO_Port, joystick_down_Pin) == GPIO_PIN_SET)
  {
    _hid_report[2] = 255;
  }

  _hid_report[3] = 0;       // data

  // joystick button 1
  if(HAL_GPIO_ReadPin(joystick_button0_GPIO_Port, joystick_button0_Pin) == GPIO_PIN_SET)
  {
    _hid_report[3] |= (1 << 0);
  }

  // joystick button 2
  if(HAL_GPIO_ReadPin(joystick_button1_GPIO_Port, joystick_button1_Pin) == GPIO_PIN_SET)
  {
    _hid_report[3] |= (1 << 1);
  }

  // joystick button 3
  if(HAL_GPIO_ReadPin(joystick_button2_GPIO_Port, joystick_button2_Pin) == GPIO_PIN_SET)
  {
    _hid_report[3] |= (1 << 2);
  }

  // joystick button 4
  if(HAL_GPIO_ReadPin(joystick_button3_GPIO_Port, joystick_button3_Pin) == GPIO_PIN_SET)
  {
    _hid_report[3] |= (1 << 3);
  }

  // joystick button 5
  // joystick button 6
  // joystick button 7
  // joystick button 8
}

static void
send_report(void)
{
  fill_report_buf();

  if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, _hid_report, 4) == USBD_BUSY)
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
