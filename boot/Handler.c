#include "stdbool.h"
#include "stdint.h"
#include "HalInterrupt.h"

__attribute__((interrupt ("IRQ"))) void Irq_handler(void) { //IRQ return address auto setting, compiler builtin feature
  Hal_interrupt_run_handler();
}

__attribute__ ((interrupt ("FIQ"))) void Fiq_handler(void) {
  while(true);
}
