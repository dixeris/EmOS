#ifndef HAL_HALINT_H
#define HAL_HALINT_H

#define INTERRUPT_HANDLER_NUM 255

typedef void (*InterHdlr_fptr)(void); //interrupt handler function pointer type definition 

void Hal_interrupt_init(void);
void Hal_interrupt_enable(uint32_t interrupt_num);
void Hal_interrupt_disable(uint32_t interrupt_num);
void Hal_interrupt_register_handler(InterHdlr_fptr handler, uint32_t interrupt_num); //connects the handler with the specific interrupt 
void Hal_interrupt_run_handler(void);

#endif //HAL_HALINT_H
