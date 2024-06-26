#include "stdint.h"
#include "memio.h"
#include "HalInterrupt.h"
#include "Interrupt.h"
#include "armcpu.h"

//defined in Regs.h
extern volatile GicCput_t *GicCpu; 
extern volatile GicDist_t *GicDist;


static InterHdlr_fptr sHandlers[INTERRUPT_HANDLER_NUM];

void Hal_interrupt_init(void) {
  GicCpu->cpucontrol.bits.Enable = 1;
  GicCpu->prioritymask.bits.Prioritymask = GIC_PRIORITY_MASK_NONE;
  GicDist->distributorctrl.bits.Enable = 1;

  for(uint32_t i = 0; i <= INTERRUPT_HANDLER_NUM; i++) {
    sHandlers[i] = NULL;
  }

  enable_irq(); //disable IRQ masking in cspr 
//  disable_fiq(); //enable FIQ masking in cspr
}

void Hal_interrupt_enable(uint32_t interrupt_num) { //enable interrupt_num interrupt 
  if((interrupt_num < GIC_IRQ_START) || (GIC_IRQ_END < interrupt_num)) {
    return;
  }

  uint32_t bit_num = interrupt_num - GIC_IRQ_START; //setenable1 -> 0~31:32~63 setenable2 -> 0~31:64~95

  if(bit_num < GIC_IRQ_START) { //interrupt num is within the setenable1 range.
    SET_BIT(GicDist->setenable1,bit_num);
  }
  else {
    bit_num -= GIC_IRQ_START;
    SET_BIT(GicDist->setenable2,bit_num);
  }

}

void Hal_interrupt_disable(uint32_t interrupt_num) { //disable interrupt_num interrupt 
  if(interrupt_num > GIC_IRQ_END || interrupt_num < GIC_IRQ_START) {
    return;
  }

  interrupt_num -= GIC_IRQ_START; //setenable1 -> 0~31:32~63 setenable2 -> 0~31:64~95

  if(interrupt_num < 32) { //interrupt num is within the setenable1 range.
    CLR_BIT(GicDist->setenable1,interrupt_num);
  }
  else {
    interrupt_num -= GIC_IRQ_START;
    CLR_BIT(GicDist->setenable2,interrupt_num);
  }

}

void Hal_interrupt_register_handler(InterHdlr_fptr handler, uint32_t interrupt_num) {
  sHandlers[interrupt_num] = handler;  
}

void Hal_interrupt_run_handler(void) {
  uint32_t interrupt_num = GicCpu->interruptack.bits.InterruptID;
  
  if(sHandlers[interrupt_num] != NULL) {
    sHandlers[interrupt_num]();
  }

  GicCpu->endofinterrupt.bits.InterruptID = interrupt_num;
}
