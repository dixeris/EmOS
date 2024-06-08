#include "stdint.h"
#include "stdio.h"
#include "HalUart.h"

static void Hw_init(void);

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
}


static void Hw_init(void) {
  Hal_uart_init();	  
}
