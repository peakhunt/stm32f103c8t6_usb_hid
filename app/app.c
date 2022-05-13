#include "stm32f1xx_hal.h"
#include "app_common.h"

#include "sys_timer.h"
#include "event_dispatcher.h"
#include "mainloop_timer.h"

#include "hid.h"
#include "misc.h"

void
app_init(void)
{
  __disable_irq();

  event_dispatcher_init();
  mainloop_timer_init();

  sys_timer_init();

  hid_init();
  misc_init();

  __enable_irq();
}

void
app_run(void)
{
  while(1)
  {
    event_dispatcher_dispatch();
  }
}
