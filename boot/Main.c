#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"

static void Hw_init(void);

static void timer_test(void);

void main(void) {
  Hw_init();

  uint32_t i = 100;
  while(i--) {
    Hal_uart_put_char('N');
  }
  Hal_uart_put_char('\n');
  putstr("Hello world\n");
 

  char *str = "debug_printf test";
  uint32_t t = 1324;
  debug_printf("output string pointer: %x\n", t);
  timer_test();
  while(true);
}


static void Hw_init(void) {
  Hal_interrupt_init();
  Hal_uart_init();	  
  Hal_timer_init();
}

static void timer_test(void) {
  while(true) {
    debug_printf("current count: %u\n", Hal_timer_get_1ms_counter());
    delay(1000);
  }
}
