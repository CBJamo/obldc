#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "printf.h"
#include "drv8305.h"
#include <stdbool.h>

void init_spi( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef  SPI_InitStruct;

  // Configure Clocks
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  // Configure GPIO
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init( GPIOB, &GPIO_InitStruct );

  GPIO_PinAFConfig( GPIOB, GPIO_PinSource3, GPIO_AF_0 );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource4, GPIO_AF_0 );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_0 );

  // Chip select
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  GPIO_ResetBits( GPIOA, GPIO_Pin_12 );

  // SPI config
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  //SPI_InitStruct.SPI_CRCPolynomial = SPI_
  SPI_Init( SPI1, &SPI_InitStruct );

  // Enable SPI
  SPI_Cmd( SPI1, ENABLE );

}

uint16_t SPISend(uint16_t data)
{
    //
    //    Setting the Data Register (DR) transmits the byte of data on MOSI.
    //
    SPI1->DR = data;
    //
    //    Wait until the data has been transmitted.
    //
    while (!(SPI1->SR & SPI_I2S_FLAG_TXE));
    //
    //    Wait for any data on MISO pin to be received.
    //
    while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
    //
    //    All data transmitted/received but SPI may be busy so wait until done.
    //
    while (SPI1->SR & SPI_I2S_FLAG_BSY);
    //
    //    Return the data received on MISO pin.
    //
    return(SPI1->DR);
}


uint16_t drv8305_transact( bool wr, uint8_t addr, uint16_t data)
{
  while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET );
  SPI_I2S_SendData16( SPI1, (wr << 15 | addr << 13 | data) );

  return SPI_I2S_ReceiveData16( SPI1 );
}

void init_drv8305( void )
{
  init_spi();

}
