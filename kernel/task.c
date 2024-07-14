#include "stdint.h"
#include "stdbool.h"
#include "switch.h"

#include "ARMv7AR.h" //CSPR "M" Mode bit 
#include "task.h"	//Task contexts 

//Init, Create
 KernelTcb_t	sTask_list[MAX_TASK_NUM];
static uint32_t		sAllocated_tcb_index;

//Scheduler Algorithm
static uint32_t		sCurrent_tcb_index;
static KernelTcb_t	*Scheduler_round_robin_algorithm(void);

//Context Switching 
 KernelTcb_t	*sCurrent_tcb;
 KernelTcb_t 	*sNext_tcb;
void	Kernel_task_scheudler(void);


//Kernel Initialization for task
void Kernel_task_init(void) {
  sAllocated_tcb_index = 0;
  sCurrent_tcb_index = 0;
  
  for(int i = 0; i <= MAX_TASK_NUM; i++) {
    sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + i * USR_TASK_STACK_SIZE);
    sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

    sTask_list[i].sp -= sizeof(KernelTaskContext_t);
    KernelTaskContext_t *ctx = (KernelTaskContext_t*)sTask_list[i].sp;
    ctx->pc = 0;
    ctx->spsr = ARM_MODE_BIT_SYS;
  }
}

//create task, sAllocated_tcb_index is number of the created task + 1
uint32_t Kernel_task_create(KernelTaskFunc_t startFunc) {
  KernelTcb_t *new_tcb = &sTask_list[sAllocated_tcb_index++];

  if(sAllocated_tcb_index > MAX_TASK_NUM) {
    return NOT_ENOUGH_TASK_NUM;
  }

  KernelTaskContext_t *ctx = (KernelTaskContext_t*)new_tcb->sp;
  ctx->pc = (uint32_t)startFunc;

  return (sAllocated_tcb_index - 1);
}

//Basic Round Robin algorithm 
static KernelTcb_t*	Scheduler_round_robin_algorithm(void) {
 sCurrent_tcb_index++;
 sCurrent_tcb_index %= sAllocated_tcb_index;
 return &sTask_list[sCurrent_tcb_index]; 
}

//Task scheduler, context switching 
void	Kernel_task_scheduler(void) {
  sCurrent_tcb	= &sTask_list[sCurrent_tcb_index];
  sNext_tcb	= Scheduler_round_robin_algorithm();

  Kernel_task_context_switching(); //ASM routine, context switching to sNext_tcb from sCurrent_tcb;
}

void Kernel_task_start(void) { //Starting from sTask_list[0] to avoid storing Kernel garbage data. This routine is called only when the kernel is starting.
  sNext_tcb = &sTask_list[sCurrent_tcb_index];
  Restore_context();  
}
