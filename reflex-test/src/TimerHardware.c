#include "TimerHardware.h"
#include "ReflexTestData.h"
#include "intervalTimer.h"

void TimerHardware_Init() {
  intervalTimer_initAll();
  TimerHardware_StartTimer();
}

double TimerHardware_GetResponseTime() {
    double responseTime = 0;
    intervalTimer_getTotalDurationInSeconds(INTERVALTIMER_TIMER0, &responseTime);
    return responseTime;
}

void TimerHardware_StopTimer() {
  intervalTimer_stop(INTERVALTIMER_TIMER0);
}

void TimerHardware_StartTimer() {
  intervalTimer_start(INTERVALTIMER_TIMER0);
}

void TimerHardware_ResetTimer(void) {
  intervalTimer_reset(INTERVALTIMER_TIMER0);
}
