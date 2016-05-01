#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

void init_uart1(uint32_t baudRate);

void uart_putch(char ch);
char uart_getch(void);
void uart_echo(void);

int uart_write(char* buff, int len);
int uart_read(char* buff, int len);
