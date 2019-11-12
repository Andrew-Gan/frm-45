#include "stm32f0xx.h"
#include "my_spi.h"
#include "stm32f0_discovery.h"
#include "int.h"
#include <stdbool.h>


void SPI_wait(int SPIX){
    if(SPIX == sel_SPI1){
        //check TXE and RXNE flag
        while((( SPI1->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0))
            ;
    }
    if(SPIX == sel_SPI2){
         //check TXE and RXNE flag
        while((SPI2->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 )
            ;
    }
}


uint8_t SPI_Send_8bit(uint8_t SPIX, uint8_t data){

    //wait for SPI to be ready
    SPI_wait(SPIX);

    if(SPIX == sel_SPI1){
        //ptr to first 8 bit of SPI1->DR
        uint8_t *ptr = (__IO uint8_t *) &(SPI1->DR);
        // put the data to the pointer
        *ptr = data;

    }

    if(SPIX == sel_SPI2){
        //ptr to first 8 bit of SPI1->DR
        uint8_t *ptr = (__IO uint8_t *) &(SPI2->DR);
        // put the data to the pointer
        *ptr = data;

    }

    //wait for SPI to be ready
    SPI_wait(SPIX);

    //if it's SPI1 return data from SPI1
    if(SPIX == sel_SPI1){
        return SPI1->DR;
    }
    return SPI2->DR;
    
}
