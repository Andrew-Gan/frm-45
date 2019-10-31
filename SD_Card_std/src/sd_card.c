
#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}



void init_spi(int SPIX, uint16_t SPI_MODE, uint16_t SPI_Direction,
uint16_t SPI_DataSize, uint16_t SPI_CPOL, uint16_t SPI_CPHA, uint16_t SPI_NSS,
uint16_t SPI_BaudRatePrescaler, uint16_t SPI_FirstBit){
    if(SPIX == SPI1){
        //enable clock for SPI1
        RCC-> APB2ENR |= RCC_APB2ENR_SPI1EN;
        //if GPIOA clock is not enable, enable it
        if((RCC->AHBENR & RCC_AHBENR_GPIOAEN) == 0){
            RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
        }
        //clear MODER for PA4,PA5,PA6,PA7
        GPIOA->MODER &= ~(3 << (2*4));
        GPIOA->MODER &= ~(3 << (2*5));
        GPIOA->MODER &= ~(3 << (2*6));
        GPIOA->MODER &= ~(3 << (2*7));
        //set it to alternate function
        GPIOA->MODER |= (2 << (2*4));
        GPIOA->MODER |= (2 << (2*5));
        GPIOA->MODER |= (2 << (2*6));
        GPIOA->MODER |= (2 << (2*7));
        
    }


    if(SPIX == SPI2){

    }


    //set SS high
    // delay 10 ms

}

void SPI_Send(){


}