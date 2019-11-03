#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"
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



void init_spi(uint8_t SPIX, uint16_t SPI_MODE, uint16_t SPI_Direction,
uint16_t SPI_DataSize, uint16_t SPI_CPOL, uint16_t SPI_CPHA, uint16_t SPI_NSS,
uint16_t SPI_BaudRatePrescaler, uint16_t SPI_SSO, bool GPIO_as_SS){
    if(SPIX == sel_SPI1){
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
        
        GPIOA->MODER |= (2 << (2*5));
        GPIOA->MODER |= (2 << (2*6));
        GPIOA->MODER |= (2 << (2*7));

        // if not choose GPIO to toggle SS
        if(!GPIO_as_SS){
           GPIOA->MODER |= (2 << (2*4));
        }
        else
        {
            GPIOA->MODER |= (1 << (2*4));
            //set SPI_SSM and SSI to high
            SPI1->CR1 |= SPI_CR1_SSI;
            SPI1->CR1 |= SPI_CR1_SSM;
            //set PA4 pin to High
            GPIOA->BSRR = GPIO_BSRR_BS_4;
        }
        

        // Set the direction
        if(SPI_Direction != 0){
          SPI1->CR1 |= SPI_Direction;
        }

        // SPI Mode
        SPI1->CR1 |= SPI_MODE;
        // set baud rate

        if(SPI_BaudRatePrescaler != 0){
            SPI1->CR1 |= SPI_BaudRatePrescaler; 
        }

        // set CPOL
        if(SPI_CPOL != 0){
            SPI1->CR1 |= SPI_CPOL;
        }

        // set CPHA
        if(SPI_CPHA != 0){
            SPI1->CR1 &= SPI_CPHA;
        }

        //set a data size
        if(SPI_DataSize != 0){
            SPI1->CR2 = SPI_DataSize;
        }

        //slave select output enable
        if(SPI_SSO != 0){
            SPI1->CR2 |= SPI_SSO;      
        }

        //automatic NSS pulse generation
        if(SPI_NSS != 0){
            SPI1->CR2 |= SPI_NSS;
        }

        // Enable the SPI1 channel by setting the SPE bit
        SPI1->CR1 |= SPI_CR1_SPE;   
    }

    if(SPIX == sel_SPI2){
       //enable clock for SPI1
        RCC-> APB1ENR |= RCC_APB1ENR_SPI2EN;
        //if GPIOB clock is not enable, enable it
        if((RCC->APB1ENR & RCC_AHBENR_GPIOBEN) == 0){
            RCC->APB1ENR |= RCC_AHBENR_GPIOBEN;
        }
        //clear MODER for PB12,PB13,PB14,PB15
        GPIOB->MODER &= ~(3 << (2*12));
        GPIOB->MODER &= ~(3 << (2*13));
        GPIOB->MODER &= ~(3 << (2*14));
        GPIOB->MODER &= ~(3 << (2*15));
        //set it to alternate function
        GPIOB->MODER |= (2 << (2*13));
        GPIOB->MODER |= (2 << (2*14));
        GPIOB->MODER |= (2 << (2*15));

        // if not choose GPIO to toggle SS
        if(!GPIO_as_SS){
        GPIOB->MODER |= (2 << (2*12));
        }
        else
        {
            GPIOB->MODER |= (1 << (2*12));
            //set SPI_SSM and SSI to high
            SPI2->CR1 |= SPI_CR1_SSI;
            SPI2->CR1 |= SPI_CR1_SSM;
            //set P12 to high
            GPIOB->BSRR = GPIO_BSRR_BS_12;
        }
        // Set the direction
        if(SPI_Direction != 0){
          SPI2->CR1 |= SPI_Direction;
        }

        // SPI Mode
        SPI2->CR1 |= SPI_MODE;
        // set baud rate

        if(SPI_BaudRatePrescaler != 0){
            SPI2->CR1 |= SPI_BaudRatePrescaler; 
        }

        // set CPOL
        if(SPI_CPOL != 0){
            SPI2->CR1 |= SPI_CPOL;
        }

        // set CPHA
        if(SPI_CPHA != 0){
            SPI2->CR1 &= SPI_CPHA;
        }

        //set a word size
        if(SPI_DataSize != 0){
            SPI2->CR2 = SPI_DataSize;
        }

        //slave select output enable
        if(SPI_SSO != 0){
            SPI2->CR2 |= SPI_SSO;      
        }

        //automatic NSS pulse generation
        if(SPI_NSS != 0){
            SPI2->CR2 |= SPI_NSS;
        }

        // Enable the SPI2 channel by setting the SPE bit
        SPI2->CR1 |= SPI_CR1_SPE;    
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
