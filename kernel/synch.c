#include "stdint.h"
#include "stdbool.h"

#define DEF_SEM_MAX 8

static int32_t Ssem;
static int32_t SsemMax;

static KernelMutex_t sMutex;

void Kernel_sem_init(int32_t max) {
  SsemMax = (max <= 0) ? DEF_SEM_MAX : max;
  SsemMax = (max >= DEF_SEM_MAX) ? DEF_SEM_MAX : max;

  Ssem = SsemMax;
}

_Bool Kernel_sem_test(void) {
  if(Ssem <= 0)
    return false;
  Ssem--;
  return true;
}


void Kernel_sem_release(void) {
  Ssem++;
  if(Ssem >= SsemMax) 
    Ssem = SsemMax;
}

void Kernel_mutex_init(void) {
  sMutex.owner = 0;
  sMutex.lock = false; 
}

_Bool Kernel_mutex_lock(uint32_t owner) {
  if(sMutex.lock) 
      return false;

  sMutex.owner = owner;
  sMutex.lock = true;
}

_Bool Kernel_mutex_unlock(uint32_t owner) {
  if(owner == sMutex.owner) {
    sMutex.lock = false;
    return true;
  }
  return false;

}
