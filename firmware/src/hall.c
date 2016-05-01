#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "hall.h"
#include "printf.h"

int Freq;


void init_hall(void)
{
  // Init structures
  GPIO_InitTypeDef         GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef        TIM_ICInitStructure;
  NVIC_InitTypeDef         NVIC_InitStructure;

  // Configure clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // enable port pins for hall inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);

  // timer base configuration
  // 126 => 3,5s till overflow ; 285,714kHz TimerClock [36MHz/Prescaler]
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  //Fill TIM_ICInit
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  //Set up interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//  TIM_ETRConfig( TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x08 );


//WTFMATE?
//  TIM_SelectHallSensor(TIM3, ENABLE);

  TIM_SelectInputTrigger( TIM3, TIM_TS_TI1F_ED );
  TIM_SelectSlaveMode( TIM3, TIM_SlaveMode_Reset );
  TIM_SelectMasterSlaveMode( TIM3, TIM_MasterSlaveMode_Enable );

  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);

  //Enable Timer
  TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  uint16_t ICValue;

  TIM_ClearITPendingBit( TIM3, TIM_IT_CC1 );

  RCC_GetClocksFreq( &RCC_Clocks );
  ICValue = TIM_GetCapture1( TIM3 );

  if( ICValue != 0)
  {
    Freq = RCC_Clocks.HCLK_Frequency / ( ICValue * 3 );
  }
  else
  {
    Freq = 0;
  }

  printf("Freq: %d\r\n", Freq);
}




