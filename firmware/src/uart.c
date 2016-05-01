#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

void init_uart1(uint32_t baudRate)
{
  // Init sctruct declaration
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;

   // Enable GPIO clock
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

   // Enable USART clock
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

   // Configure pin alternate functions
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

   // Configure USART Tx, Rx as alternate function push-pull
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   // UART init
   USART_InitStructure.USART_BaudRate = baudRate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART1, &USART_InitStructure);

   // Enable USART
   USART_Cmd(USART1, ENABLE);

}

void uart_putch( char ch )
{
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, ch);

  return;
}

char uart_getch( void )
{
  while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  return (uint8_t)USART_ReceiveData(USART1);
}

void uart_echo( void )
{
  char tmp;

  tmp = uart_getch();
  uart_putch( tmp );

}

int uart_write(char* buff, int len)
{
  int i;
  for( i = 0; i < len; i++ )
  {
    uart_putch(buff[i]);
  }

  return len;
}

int uart_read(char* buff, int len)
{
  int i;
  for( i = 0; i < len; i++ )
  {
    buff[i] = uart_getch();
  }

  return len;
}
