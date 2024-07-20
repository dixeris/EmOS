#ifndef KERNEL_MSG_H
#define KERNEL_MSG_H

#define MSG_Q_SIZE_BYTE 512

typedef enum KernelMsgQ_t {
  KernelMsgQ_Task0,
  KernelMsgQ_Task1,
  KernelMsgQ_Task2,
  
  KernelMsgQ_Num

} KernelMsgQ_t;

typedef struct KernelCirQ_t {
  uint8_t Queue[MSG_Q_SIZE_BYTE];
  uint32_t front;
  uint32_t rear;
} KernelCirQ_t;

void Kernel_msgQ_init(void);
_Bool Kernel_msgQ_is_empty(KernelMsgQ_t);
_Bool Kernel_msgQ_is_full(KernelMsgQ_t);
_Bool Kernel_msgQ_enqueue(KernelMsgQ_t, uint8_t);
_Bool Kernel_msgQ_dequeue(KernelMsgQ_t, uint8_t*);
#endif 
