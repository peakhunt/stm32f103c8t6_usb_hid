#ifndef __MAINLOOP_TIMER_DEF_H__
#define __MAINLOOP_TIMER_DEF_H__

#include "app_common.h"
#include "soft_timer.h"

extern void mainloop_timer_init(void);
extern void mainloop_timer_schedule(SoftTimerElem* elem, uint32_t expires);
extern void mainloop_timer_cancel(SoftTimerElem* elem);

#endif //!__MAINLOOP_TIMER_DEF_H__
