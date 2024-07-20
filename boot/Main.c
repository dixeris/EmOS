#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "Kernel.h"

static void Hw_init(void);
static void Kernel_init(void);
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
 
 Kernel_init(); 
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
  if (NOT_ENOUGH_TASK_NUM == taskId) {
    putstr("Task0 creation failed\n");
  }
 
  taskId = Kernel_task_create(User_task1);
  if (NOT_ENOUGH_TASK_NUM == taskId) {
    putstr("Task1 creation failed\n");
  }

//  taskId = Kernel_task_create(User_task2);
//  if (NOT_ENOUGH_TASK_NUM == taskId) {
//    putstr("Task2 creation failed\n");
//  }

  Kernel_start();
}

void User_task0(void) {
  uint32_t local = 0;
  debug_printf("User Task #%u\n", local);

  uint8_t cmdBuf[16];
  uint32_t cmdBufIdx = 0;
  uint8_t uartch = 0;

  while(true) {
    KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_R|KernelEventFlag_N);
    switch(handle_event) {
      case KernelEventFlag_UartIn:
	Kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);
	if(uartch == '\r') { //if "enter" 
	  cmdBuf[cmdBufIdx] = '\0';

	  Kernel_send_msg(KernelMsgQ_Task1, &cmdBufIdx, 1);
	  Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, cmdBufIdx);
	  Kernel_send_events(KernelEventFlag_CmdIn);

	  cmdBufIdx = 0;
	}

	else {
	  cmdBuf[cmdBufIdx] = uartch;
	  cmdBufIdx++;
	  cmdBufIdx %= 16;
	}
	break;     
	
    }
	  Kernel_yield();
  }
}

void User_task1(void) {
  uint32_t local = 1;
  debug_printf("User Task #%u\n", local);

  uint8_t cmdBuf[16];
  uint32_t cmdlen;
  
  while(true) {
    KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn);
    switch(handle_event) {
      case KernelEventFlag_CmdIn:
	memclr(cmdBuf, 16);
	Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen, 1);
	debug_printf("cmdlen = %u\n", cmdlen);
	Kernel_recv_msg(KernelMsgQ_Task1, cmdBuf, cmdlen);
	debug_printf("\nRecv cmd: %s\n", cmdBuf );
	break;      
    }
	  Kernel_yield();
  }
}

void User_task2(void) {
  uint32_t local = 2;
  debug_printf("User Task #%u\n", local);

  while(true) {
    KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn);
    switch(handle_event) {
      case KernelEventFlag_UartIn:
	break;      
    }
	  Kernel_yield();
  }
}
