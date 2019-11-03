
#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"
#include "my_spi.h"
#include "stm32f0_discovery.h"
#include "int.h"
#include "timer.h"
#include <stdbool.h>


static bool wait_for_card_ready(unsigned int timeout){
    BYTE data;
    Timer2 = timeout;
    do{
      data = SPI_Send_8bit(sel_SPI1,0xFF);
    } while(data != 0xFF && Timer2);

    return (data == 0xFF ? 1 : 0);
}

static void deselect_card(){
    //set PA4 to be high
    GPIOA->BSRR = GPIO_BSRR_BS_4;
    //dummy clock
    SPI_Send_8bit(sel_SPI1,0xFF);

}

static bool select_card(){
    //set PA4 to be low
    GPIOA->BSRR = GPIO_BSRR_BR_4;
    //dummy clock
    SPI_Send_8bit(sel_SPI1,0xFF);
    //wait for SD card to be ready
    if(wait_for_card_ready(500)){
        return true;
    }
    deselect_card();
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
	SPI_Send_8bit(sel_SPI1, 0x40 | cmd);			/* add 0 1 to command */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 24));		/* Argument[31..24] */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 16));		/* Argument[23..16] */
	SPI_Send_8bit(sel_SPI1, (BYTE)(arg >> 8));		/* Argument[15..8] */
	SPI_Send_8bit(sel_SPI1, (BYTE)arg);				/* Argument[7..0] */										/* Dummy CRC + Stop */
	//send optional CRC
	n = 0x01;                           /* Dummy CRC + Stop */
	    if (cmd == CMD0) n = 0x95;          /* Valid CRC for CMD0(0) */
	    if (cmd == CMD8) n = 0x87;          /* Valid CRC for CMD8(0x1AA) */
	SPI_Send_8bit(sel_SPI1,(BYTE)n);
	

	/* Receive command resp */
	if (cmd == CMD12) {
		SPI_Send_8bit(sel_SPI1, 0xFF);					/* Discard following one byte when CMD12 */
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
    uint16_t SPI_DataSize =  SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 |  SPI_CR2_FRXTH; //so it set to 8 bit
    uint16_t SPI_CPOL = 0; //proper setting for SD read
    uint16_t SPI_CPHA = 0; //proper setting for SD read
    uint16_t SPI_NSS = 0; //disable NSS
    uint16_t SPI_BaudRatePrescaler =  SPI_CR1_BR_1 | SPI_CR1_BR_2; //use lowest frequency
    uint16_t SPI_SSO = 0;
    bool GPIO_as_SS = true; //use GPIO to toggle
    init_spi(SPIX, SPI_MODE, SPI_Direction, SPI_DataSize, SPI_CPOL,
    SPI_CPHA,SPI_NSS,SPI_BaudRatePrescaler,SPI_SSO,GPIO_as_SS);
    
    //initialize timer
     volatile uint16_t prescalar = 480;
     volatile uint16_t arr = 100;
     init_tim6(prescalar, arr);



    //send  80 dummy value
    for (int i = 0; i < 10; i++){
        SPI_Send_8bit(sel_SPI1,0xFF);
    }


    BYTE test = send_cmd(CMD0,0);

    return 0;
}



