#ifndef KERNEL_SEM_H
#define KERNEL_SEM_H

void Kernel_sem_init(int32_t);
_Bool Kernel_sem_test(void);
_Bool Kernel_sem_release(void);

typedef struct KernelMutex_t {
  uint32_t owner;
  _Bool lock;
} KernelMutex_t;

void Kernel_mutex_init(void);
_Bool Kernel_mutex_lock(uint32_t owner);
_Bool Kernel_mutex_unlock(uint32_t owner);

#endif 
