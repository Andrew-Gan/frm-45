#include "stm32f0xx.h"
#include "int.h"
#include <stdbool.h>

#define sel_SPI1 1
#define sel_SPI2 2

/*
input: int SPI1  = selSPI1
       int SPI2  = selSPI2
function: wait until SPI is ready
*/
void SPI_wait(int SPIX);


/*
input: SPI1  = selSPI1
       SPI2  = selSPI2
       data = 8 bit data
function: Send 8 Bit to selected SPI
*/
uint8_t SPI_Send_8bit(uint8_t SPIX, uint8_t data);
