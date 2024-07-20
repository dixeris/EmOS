#include "stdint.h"
#include "memio.h"
#include "stdbool.h"
#include "Kernel.h"

void Kernel_yield(void) {
  Kernel_task_scheduler();
}

void Kernel_start(void) {
  Kernel_task_start();
}

void Kernel_send_events(uint32_t eventlist) {
  for(uint32_t i = 0; i < 32; i++) {
    if((eventlist >> i) & 1){
      KernelEventFlag_t sendingEvent = KernelEventFlag_Empty;
      sendingEvent = (KernelEventFlag_t)SET_BIT(sendingEvent, i);
      Kernel_event_flag_set(sendingEvent);
    }
  }
}

KernelEventFlag_t Kernel_wait_events(uint32_t eventlist) {
  for(uint32_t i = 0; i < 32; i++) {
    if((eventlist >> i & 1)) {
      KernelEventFlag_t waitingEvent = KernelEventFlag_Empty;
      waitingEvent = (KernelEventFlag_t)SET_BIT(waitingEvent,i);
      if(Kernel_event_flag_check(waitingEvent)) {
	return waitingEvent;
      }

	  
    }
  }
  return KernelEventFlag_Empty;

}

_Bool Kernel_send_msg(KernelMsgQ_t Qname, void* data, uint32_t count) {
  uint8_t* d = (uint8_t*)data;

  for(uint32_t i = 0; i < count; i++) {

    if(Kernel_msgQ_enqueue(Qname, *d) == false) {
      for(uint32_t j = 0; j < i; j++) {
	uint8_t rollback;
	Kernel_msgQ_dequeue(Qname, &rollback);
      }
      return false;
    }
    d++;
  }
  return true;
}

uint32_t Kernel_recv_msg(KernelMsgQ_t Qname, void* out_data, uint32_t count) {
  uint8_t* d = (uint8_t*)out_data;

  for(uint32_t i = 0; i < count; i++) {

    if(Kernel_msgQ_dequeue(Qname, d) == false) {
      return i;
    }
    d++;
  }
  return count;
}
