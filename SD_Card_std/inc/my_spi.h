#include "stm32f0xx.h"
#include "int.h"
#include <stdbool.h>


/*
input: int SPI1  = selSPI1
       int SPI2  = selSPI2
function: wait until SPI is ready
*/
void SPI_wait(int SPIX);


/*
input: put 0 as default
function : initialize SPI
//demo of how to init SPI
  uint8_t SPIX = 1;
  uint16_t SPI_MODE = SPI_CR1_MSTR;
  uint16_t SPI_Direction = 0; // 2 line unidirectional data
  uint16_t SPI_DataSize = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; //so it set to 8 bit
  uint16_t SPI_CPOL = 0; //proper setting for SD read
  uint16_t SPI_CPHA = 0; //proper setting for SD read
  uint16_t SPI_NSS = 0; //disable NSS
  uint16_t SPI_BaudRatePrescaler = SPI_CR1_BR_0; //use second max frequency
  uint16_t SPI_SSO = 0;
  bool GPIO_as_SS = true; //use GPIO to toggle
  init_spi(SPIX, SPI_MODE, SPI_Direction, SPI_DataSize, SPI_CPOL,
  SPI_CPOL,SPI_NSS,SPI_BaudRatePrescaler,SPI_SSO,GPIO_as_SS);
*/

void init_spi(uint8_t SPIX, uint16_t SPI_MODE, uint16_t SPI_Direction,
uint16_t SPI_DataSize, uint16_t SPI_CPOL, uint16_t SPI_CPHA, uint16_t SPI_NSS,
uint16_t SPI_BaudRatePrescaler, uint16_t SPI_SSO, bool GPIO_as_SS);

/*
input: SPI1  = selSPI1
       SPI2  = selSPI2
       data = 8 bit data
function: Send 8 Bit to SPI
*/
uint8_t SPI_Send_8bit(uint8_t SPIX, uint8_t data);
