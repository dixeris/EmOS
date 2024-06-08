#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t *Uart; //defined in Regs.c

void Hal_uart_init(void) {
  // Enable Uart
  Uart->uartcr.bits.UARTEN = 0; //disable UART
  Uart->uartcr.bits.TXE = 1;  //enable Transmit
  Uart->uartcr.bits.RXE = 1;  //enable Receive
  Uart->uartcr.bits.UARTEN = 1;  //re-enbale UART
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
