
#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"
#include "stm32f0_discovery.h"
#include "int.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void SPI_wait(int SPIX){
    if(SPIX == sel_SPI1){
        //check TXE and RXNE flag
        while((SPI1->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPI1)->SR & SPI_SR_BSY) );
    }
    if(SPIX == sel_SPI2){
         //check TXE and RXNE flag
        while((SPI2->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPI2)->SR & SPI_SR_BSY));
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

    //send data to SPI_DR
    if(SPIX == sel_SPI1){
        SPI1 -> DR = data;
    }

    if(SPIX == sel_SPI2){
        SPI2 -> DR = data;
    }
    SPI_wait(SPIX);

    //if it's SPI1 return data from SPI1
    if(SPIX == sel_SPI1){
        return SPI1->DR;
    }
    return SPI2->DR;
    
}

static bool wait_for_card_ready(){
    BYTE data;
    do{
      data = SPI_Send_8bit(sel_SPI1,0xFF);
    } while(data != 0xFF);
    if(data == 0xFF){
        return true;
    }
    return false;
}

static void deselect_card(){
    //dummy clock
    SPI_Send_8bit(sel_SPI1,0xFF);
    //set PA4 to be high
    GPIOA->BSRR = GPIO_BSRR_BS_4;
}

static bool select_card(){
    //set PA4 to be low
    GPIOA->BSRR = GPIO_BSRR_BR_4;
    //dummy clock
    SPI_Send_8bit(sel_SPI1,0xFF);
    //wait for SD card to be ready
    if(wait_for_card_ready() == true){
        return true;
    }
    return false;
}

static BYTE send_cmd(BYTE cmd, DWORD arg){
    BYTE n, res;
	
	if (cmd & 0x80) {	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		deselect_card();
        if(!select_card()) return 0xFF;
	}

	/* Send command packet */
	SPI_Send_8bit(sel_SPI1, 0x40 | cmd);				/* Start + command index */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 24));		/* Argument[31..24] */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 16));		/* Argument[23..16] */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 8));		/* Argument[15..8] */
	SPI_Send_8bit(sel_SPI1, (BYTE)arg);				/* Argument[7..0] */
	n = 0x01;										/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;						/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;						/* Valid CRC for CMD8(0x1AA) */
	SPI_Send_8bit(sel_SPI1, n);

	/* Receive command resp */
	if (cmd == CMD12) {
		SPI_Send_8bit(sel_SPI1, 0xFF);					/* Diacard following one byte when CMD12 */
	}
	
	n = 10;								/* Wait for response (10 bytes max) */
	do {
		res = SPI_Send_8bit(sel_SPI1, 0xFF);
	} while ((res & 0x80) && --n);

	return res;							/* Return received response */
}

DSTATUS SD_initialize (){
    BYTE n, cmd, ty, ocr[4];

    //initialize SPI1
    uint8_t SPIX = 1;
    uint16_t SPI_MODE = SPI_CR1_MSTR;
    uint16_t SPI_Direction = 0; // 2 line unidirectional data
    uint16_t SPI_DataSize = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; //so it set to 8 bit
    uint16_t SPI_CPOL = 0; //proper setting for SD read
    uint16_t SPI_CPHA = 0; //proper setting for SD read
    uint16_t SPI_NSS = 0; //disable NSS
    uint16_t SPI_BaudRatePrescaler =  SPI_CR1_BR_1 | SPI_CR1_BR_2; //use lowest frequency
    uint16_t SPI_SSO = 0;
    bool GPIO_as_SS = true; //use GPIO to toggle
    init_spi(SPIX, SPI_MODE, SPI_Direction, SPI_DataSize, SPI_CPOL,
    SPI_CPOL,SPI_NSS,SPI_BaudRatePrescaler,SPI_SSO,GPIO_as_SS);

    //wait 10ms
    nano_wait(10 * milisecond);

    //send dummy value
    for (int i = 0; i < 10; i++){
        SPI_Send_8bit(sel_SPI1,0xFF);
    }

    int test = send_cmd(CMD0,0);


    return 0;

}
