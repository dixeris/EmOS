#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "Kernel.h"

extern volatile PL011_t *Uart; //defined in Regs.c

static void interrupt_handler(void);

void Hal_uart_init(void) {
  // Enable Uart
  Uart->uartcr.bits.UARTEN = 0; //disable UART
  Uart->uartcr.bits.TXE = 1;  //enable Transmit
  Uart->uartcr.bits.RXE = 1;  //enable Receive
  Uart->uartcr.bits.UARTEN = 1;  //re-enbale UART

  //Enable input interrupt
  Uart->uartimsc.bits.RXIM = 1; //Interrutpt Mask Set/Clear bit, Receive Interrupt Mask 
 // Uart->uartimsc.bits.TXIM = 1; //Interrutpt Mask Set/Clear bit, Receive Interrupt Mask 

  //Register UART interrupt 
  Hal_interrupt_enable(UART_INTERRUPT0);
  Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch) {
  while(Uart->uartfr.bits.TXFF); //wait until the FIFO buffer is empty 
  Uart->uartdr.all = (ch & 0xFF); //write to Data bit in data register size of 8 bit 
}


uint8_t  Hal_uart_get_char(void) {
  uint8_t data;

  while(Uart->uartfr.bits.RXFE);

  data = Uart->uartdr.all;

  //Check for an errfor flag
  if(data & 0xFFFFFF00) 
  {
    Uart->uartrsr.all = 0xFF;
    return 0;
  }
  return (uint8_t)(data & 0xFF);
}

static void interrupt_handler(void) {
  uint8_t ch = Hal_uart_get_char();
  Hal_uart_put_char(ch);


  Kernel_send_msg(KernelMsgQ_Task0, &ch, 1); //sending msg to Taks0 msg Queue
  Kernel_send_events(KernelEventFlag_UartIn); //setting event flag 
}
