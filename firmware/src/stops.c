#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "uart.h"
#include "stops.h"

void init_stop_irq( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  // Configure Clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Configure GPIO
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init( GPIOC, &GPIO_InitStruct );

  // Configure Interupts
  SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOC, EXTI_PinSource13 );
  SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOC, EXTI_PinSource14 );
  SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOC, EXTI_PinSource15 );

  EXTI_InitStruct.EXTI_Line = EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init( &EXTI_InitStruct );

  // Add Interupts to NVIC
  NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStruct );
}

void EXTI4_15_IRQHandler( void )
{
  if (EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    // Clockwise Endstop
    uart_write( "cw end\r\n", 8);

    EXTI_ClearITPendingBit(EXTI_Line13);
  }
  if (EXTI_GetITStatus(EXTI_Line14) != RESET)
  {
    // Counter Clockwise Endstop
    uart_write( "ccw end\r\n", 9);

    EXTI_ClearITPendingBit(EXTI_Line14);
  }
  if (EXTI_GetITStatus(EXTI_Line15) != RESET)
  {
    // Emergency Stop
    uart_write( "estop\r\n", 7);

    EXTI_ClearITPendingBit(EXTI_Line15);
  }
}

