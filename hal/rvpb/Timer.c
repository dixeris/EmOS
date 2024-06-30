#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"

extern volatile Timer_t* Timer;

static uint32_t internal_1ms_counter;

static void interrupt_handler(void);

void Hal_timer_init(void) {

  //interface reset
  Timer->timerxctrl.bits.OneShot = 0;
  Timer->timerxctrl.bits.TimerSize = 0;
  Timer->timerxctrl.bits.TimerPre = 0;
  Timer->timerxctrl.bits.IntEnable = 1;
  Timer->timerxctrl.bits.TimerMode = 0;
  Timer->timerxctrl.bits.TimerEn = 0;

  Timer->timerxload = 0;
  Timer->timerxvalue = 0xFFFFFFFF;

  //periodic setting
  Timer->timerxctrl.bits.TimerMode = TIMER_PERIODIC;		//Timer mode is Periodic  
  Timer->timerxctrl.bits.TimerSize = TIMER_32BIT_COUNTER;	//Timer size 32 bit 
  Timer->timerxctrl.bits.OneShot = 0;	//wrapping mode, not oneshot mode 
  Timer->timerxctrl.bits.TimerPre = 0;	//Timer Prescale is 1 
  Timer->timerxctrl.bits.IntEnable = 1;	//Interrupt Enable 

  uint32_t interval_1ms = TIMER_1MZ_INTERVAL / 1000;

  Timer->timerxload = interval_1ms;
  Timer->timerxctrl.bits.TimerEn = 1;

  internal_1ms_counter = 0;
  
  //Register timer interrupt handler 
  Hal_interrupt_enable(TIMER_INTERRUPT);
  Hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT); 
}

static void interrupt_handler(void) {
  internal_1ms_counter++;
  Timer->timerxintclr = 1;
}

uint32_t Hal_timer_get_1ms_counter(void) {
  return internal_1ms_counter;
}
