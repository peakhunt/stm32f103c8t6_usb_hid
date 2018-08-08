#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_custom_hid_if.h"
#include "usb_gpio_report_queue.h"

#define MAX_SEND_NDX    128

#define REPORT_SEND_IDLE        0
#define REPORT_SEND_BUSY        1

static volatile uint8_t   _report_write_pos   = 0;
static volatile uint8_t   _report_read_pos    = 0;
static volatile uint8_t   _report_count       = 0;
static volatile uint8_t SendBuffer[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE * MAX_SEND_NDX];
static uint8_t USBTXBuffer[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 1];
static uint8_t   report_state = REPORT_SEND_IDLE;

static uint16_t  btn_list[] =
{
  joystick_button0_Pin,
  joystick_button1_Pin,
  joystick_button2_Pin,
  joystick_button3_Pin,
  GPIO_PIN_10,
  GPIO_PIN_11,
};

static GPIO_TypeDef* btn_port_list[] =
{
  joystick_button0_GPIO_Port,
  joystick_button1_GPIO_Port,
  joystick_button2_GPIO_Port,
  joystick_button3_GPIO_Port,
  joystick_button3_GPIO_Port,
  joystick_button3_GPIO_Port,
};

//
// XXX called from ISR
//
static void
enqueu_usb_report(uint8_t x, uint8_t y, uint8_t buttons)
{
  volatile uint8_t*    send_buffer;

  if(_report_count >= MAX_SEND_NDX)
  {
    // overflow
    return;
  }

  send_buffer = &SendBuffer[_report_write_pos * USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];
  _report_write_pos = (_report_write_pos + 1) % MAX_SEND_NDX;
  _report_count++;

  send_buffer[0] = x;
  send_buffer[1] = y;
  send_buffer[2] = buttons;
}

//
// XXX called from main loop
//
static uint8_t
dequeue_usb_report(uint8_t* x, uint8_t* y, uint8_t* buttons)
{
  __disable_irq();
  if(_report_count == 0)
  {
    __enable_irq();
    return 0;
  }

  *x        = SendBuffer[_report_read_pos * USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 0];
  *y        = SendBuffer[_report_read_pos * USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 1];
  *buttons  = SendBuffer[_report_read_pos * USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 2];

  _report_read_pos = (_report_read_pos + 1) % MAX_SEND_NDX;
  _report_count--;

  __enable_irq();
  return 1;
}

void
usb_gpio_report_queue_do_work(void)
{
  switch(report_state)
  {
    case REPORT_SEND_IDLE:
      USBTXBuffer[0] = 1;   // report ID
      if(dequeue_usb_report(&USBTXBuffer[1], &USBTXBuffer[2], &USBTXBuffer[3]) == 1)
      {
        if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USBTXBuffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 1) != USBD_OK)
        {
          report_state = REPORT_SEND_BUSY;
        }
      }
      break;

    case REPORT_SEND_BUSY:
      if(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USBTXBuffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE + 1) == USBD_OK)
      {
        report_state = REPORT_SEND_IDLE;
      }
      break;
  }
}

void
handle_gpio_irq(void)
{
  GPIO_PinState ps;
  uint8_t       i,
                x,
                y,
                buttons;

  x       = 128;      // X
  y       = 128;      // Y
  buttons = 0;        // Button State

  if(HAL_GPIO_ReadPin(joystick_left_GPIO_Port, joystick_left_Pin) == GPIO_PIN_RESET)
  {
    x = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_right_GPIO_Port, joystick_right_Pin) == GPIO_PIN_RESET)
  {
    x = 255;
  }

  if(HAL_GPIO_ReadPin(joystick_up_GPIO_Port, joystick_up_Pin) == GPIO_PIN_RESET)
  {
    y = 0;
  }
  else if(HAL_GPIO_ReadPin(joystick_down_GPIO_Port, joystick_down_Pin) == GPIO_PIN_RESET)
  {
    y = 255;
  }

  for(i = 0; i < sizeof(btn_list)/sizeof(uint16_t); i++)
  {
    ps = HAL_GPIO_ReadPin(btn_port_list[i], btn_list[i]);

    if(ps == GPIO_PIN_RESET)
    {
      buttons |= (1 << i);
    }
  }

  enqueu_usb_report(x, y, buttons);
}
