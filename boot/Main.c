#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "task.h"

static void Hw_init(void);

static void timer_test(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);

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

static void Kernel_init(void) {
  uint32_t taskId;

  Kernel_task_init();

  taskId = Kernel_task_create(User_task0);
  if(NOT_ENOUGH_TASK_NUM == taskId) {
    putstr("Task1 creation fail\n");
  }

  taskId = Kernel_task_create(User_task1);
  if(NOT_ENOUGH_TASK_NUM == taskId) {
    putstr("Task1 creation fail\n");
  }

  taskId = Kernel_task_create(User_task2);
  if(NOT_ENOUGH_TASK_NUM == taskId) {
    putstr("Task1 creation fail\n");
  }
}

void User_task0(void) {
  debug_printf("User Task #0\n");
  while(true);
}
void User_task1(void) {
  debug_printf("User Task #1\n");
  while(true);
}
void User_task2(void) {
  debug_printf("User Task #2\n");
  while(true);
}
