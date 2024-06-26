#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"
#include "HalUart.h"

#define PRINTF_BUF_LEN 1024

static char printf_buf[PRINTF_BUF_LEN]; //1KB 

uint32_t putstr(const char *s) {
  uint32_t c = 0;
  while(*s) {
    Hal_uart_put_char(*s++);
    c++;
  }
  return c;
}

uint32_t debug_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsprintf(printf_buf, format, args);
  va_end(args);

  return putstr(printf_buf);
}

uint32_t vsprintf(char *buf, const char *format, va_list args) {
  uint32_t c = 0;

  char ch;
  char *str;
  
  uint32_t uint;
  uint32_t hex;

  for(uint32_t i = 0; format[i]; i++) {
    if(format[i] == '%') {
      i++;
      switch(format[i]) {
	case 'c':
	  ch  = (char)va_arg(args, int32_t);
	  buf[c++] = ch;
	  break;
	case 's': 
	  str = (char*)va_arg(args, char*);
	  if (str == NULL) {
	    str = "(null)";
	  }
	  while(*str) {
	  	buf[c++] = (*str++);
	  }
	  break;
	case 'u': //convert unsigned into readable string 
	  uint = (uint32_t)va_arg(args, uint32_t);
	  c += utoa(&buf[c], uint, utoa_dec);
	  break;
	case 'x':
	  hex = (uint32_t)va_arg(args, uint32_t);
	  c += utoa(&buf[c], hex, utoa_hex);
	  break;
      }      
    }
    else {
      buf[c++] = format[i];
    }
  
  if(c >= PRINTF_BUF_LEN) {
    buf[i] = '\0';
    return 0;
  }
  }

  buf[c] = '\0';
  return c;
}

uint32_t utoa(char *buf, uint32_t val, utoa_t base)
{
  uint32_t c = 0;
  int32_t idx = 0;
  char tmp[11];

  do //만약 val 값이 0 이라면 이 루프는 실행조차 되지 않는 것을 방지하기 위해서 do while 문을 사용함.
  {
  uint32_t t = val % (uint32_t)base;   //extract first digit, if the base is 16, it is possible to extract the number over 10.
  if(t >= 10) {
    t = t - 10 + 'A' - '0'; // 12 - 10 + 65 -> 67 in ascii code, it is converted to letter "C" which is hex, -0 for skipping next step 
  }

  tmp[idx] = (t + '0'); //if the extracted number is lower than 10, it should be converted to decimal letter which is represented from number 48('0').
  val /= base; //jump into next digit 
  idx++;
  } while(val);

 idx--; 
 while(idx >= 0)  {
   buf[c++] = tmp[idx];
   idx--;
 } 
 
 return c;   
}

  
