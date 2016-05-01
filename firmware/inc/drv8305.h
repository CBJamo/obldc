#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include <stdbool.h>

void init_spi( void );

uint16_t SPISend( uint16_t data );

uint16_t drv8305_transact( bool wr, uint8_t addr, uint16_t data);
