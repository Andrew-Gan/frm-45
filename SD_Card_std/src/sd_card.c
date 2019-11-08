
#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"
#include "my_spi.h"
#include "stm32f0_discovery.h"
#include "int.h"
#include "timer.h"
#include <stdbool.h>

static BYTE CardType;			/* Card type flags */
static volatile DSTATUS Stat = STA_NOINIT;	/* Physical drive status */

/* Receive multiple byte */
static void rcvr_spi_multi (
	BYTE *buff,		/* Pointer to data buffer */
	UINT btr		/* Number of bytes to receive (even number) */
)
{
	WORD d;
 
    // wait until SPI transmit finish
    SPI_wait(sel_SPI1);
    // turn off SPI
    SPI1->CR1 &= ~SPI_CR1_SPE;
    //clear DS bit
    SPI1->CR2 &= ~(SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /* Put SPI into 16-bit mode */
    SPI1->CR2 |= (SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /*set FRXTH to 0 (16 bit)*/
    SPI1->CR2 &= ~SPI_CR2_FRXTH;
    // turn on SPI
    SPI1->CR1 |= SPI_CR1_SPE;


	SPI1->DR  = 0xFFFF;		/* Start the first SPI transaction */
	btr -= 2;
	do {					/* Receive the data block into buffer */
		SPI_wait(sel_SPI1);/* Wait for end of the SPI transaction */
		d = SPI1 -> DR;		/* Get received word */
		SPI1 -> DR = 0xFFFF;/* Start next transaction */
		buff[1] = d;
		buff[0] = d >> 8; 		/* Store received data */
		buff += 2;
	} while (btr -= 2);
	SPI_wait(sel_SPI1);		/* Wait for end of the SPI transaction */
	d = SPI1->DR;			/* Get last word received */
	buff[1] = d;
	buff[0] = d >> 8;		/* Store it */


	// wait until SPI transmit finish
	SPI_wait(sel_SPI1);
	// turn off SPI
	SPI1->CR1 &= ~SPI_CR1_SPE;
    //clear DS bit
    SPI1->CR2 &= ~(SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /* Put SPI into 8-bit mode */
    SPI1->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /*set FRXTH to 1 */
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // turn on SPI
    SPI1->CR1 |= SPI_CR1_SPE;

}

////////////////////////////////////////////////////////////////////////////////

#if FF_FS_READONLY == 0
/* Send multiple byte */
static void xmit_spi_multi (
    const BYTE *buff,   /* Pointer to the data */
    UINT btx            /* Number of bytes to send (even number) */
)
{
    WORD d;

    // wait until SPI transmit finish
    SPI_wait(sel_SPI1);
    // turn off SPI
    SPI1->CR1 &= ~SPI_CR1_SPE;
    //clear DS bit
    SPI1->CR2 &= ~(SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /* Put SPI into 16-bit mode */
    SPI1->CR2 |= (SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /*set FRXTH to 0 (16 bit)*/
    SPI1->CR2 &= ~SPI_CR2_FRXTH;
    // turn on SPI
    SPI1->CR1 |= SPI_CR1_SPE;


    d = buff[0] << 8 | buff[1]; buff += 2;
    SPI1 -> DR = d;    /* Send the first word */
    btx -= 2;
    do {
        d = buff[0] << 8 | buff[1]; buff += 2;  /* Word to send next */
        SPI_wait(sel_SPI1);  /* Wait for end of the SPI transaction */
        SPI1->DR;                            /* Discard received word */
        SPI1->DR = d;                        /* Start next transaction */
    } while (btx -= 2);
    SPI_wait(sel_SPI1); ;  /* Wait for end of the SPI transaction */
    SPI1->DR;                            /* Discard received word */

    // wait until SPI transmit finish
    SPI_wait(sel_SPI1);
    // turn off SPI
    SPI1->CR1 &= ~SPI_CR1_SPE;
    //clear DS bit
    SPI1->CR2 &= ~(SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /* Put SPI into 8-bit mode */
    SPI1->CR2 |= (SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0);
    /*set FRXTH to 1 */
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // turn on SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}
#endif


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



/*-----------------------------------------------------------------------*/
/* Receive a data packet from the MMC                                    */
/*-----------------------------------------------------------------------*/

static int rcvr_datablock (    /* 1:OK, 0:Error */
    BYTE *buff,         /* Data buffer */
    UINT btr            /* Data block length (byte) */
)
{
    BYTE token;


    Timer1 = 200;
    do {                            /* Wait for DataStart token in timeout of 200ms */
        token =  SPI_Send_8bit(sel_SPI1,0xFF);
        /* This loop will take a time. Insert rot_rdq() here for multitask envilonment. */
    } while ((token == 0xFF) && Timer1);
    if(token != 0xFE) return 0;     /* Function fails if invalid DataStart token or timeout */

    rcvr_spi_multi(buff, btr);      /* Store trailing data to the buffer */
    SPI_Send_8bit(sel_SPI1,0xFF);
    SPI_Send_8bit(sel_SPI1,0xFF);   /* Discard CRC */

    return 1;                       /* Function succeeded */
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
static
int xmit_datablock (    /* 1:OK, 0:Failed */
    const BYTE *buff,   /* Ponter to 512 byte data to be sent */
    BYTE token          /* Token */
)
{
    BYTE resp;


    if (!wait_for_card_ready(500)) return 0;     /* Wait for card ready */

    SPI_Send_8bit(sel_SPI1,token);                   /* Send token */
    if (token != 0xFD) {                /* Send data if token is other than StopTran */
        xmit_spi_multi(buff, 512);      /* Data */
        SPI_Send_8bit(sel_SPI1,0xFF);
        SPI_Send_8bit(sel_SPI1,0xFF); /* Dummy CRC */

        resp = SPI_Send_8bit(sel_SPI1,0xFF);    /* Receive data resp */
        if ((resp & 0x1F) != 0x05) return 0;    /* Function fails if the data packet was not accepted */
    }
    return 1;
}
#endif

/*-----------------------------------------------------------------------*/
/* Send a command to the MMC                                         */
/*-----------------------------------------------------------------------*/
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

DSTATUS disk_status (
	BYTE drv		/* Physical drive number (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only drive 0 */

	return Stat;	/* Return disk status */
}


DSTATUS disk_initialize (BYTE drv){
    BYTE n, cmd, ty, ocr[4];

    if (drv) return STA_NOINIT;

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

    ty = 0;
    BYTE test = send_cmd(CMD0,0);
    if(test == 1){
        Timer1 = 1000;
        test = send_cmd(CMD8, 0x1AA);
        if(test == 1){

            //receive 32 bit
            for(n = 0; n<4; n++) { ocr[n] = SPI_Send_8bit(sel_SPI1,0xFF);}

            /* Is the card supports Vcc of 2.7-3.6V? */
            if(ocr[2] == 1 && ocr[3] == 0xAA){
                test = send_cmd(ACMD41, 1UL << 30);
                while (Timer1 && test){
                    test = send_cmd(ACMD41, 1UL << 30);
                    if(Timer1 == 0) {return 1;}
                }
                //finish the initialization process
                test = send_cmd(CMD58, 0);
            	if((Timer1 && test) == 0){              //read OCR
            		//receive 32 bit
                    for(n = 0; n<4; n++){  ocr[n] = SPI_Send_8bit(sel_SPI1,0xFF);}
                    ty = (ocr[0] & 0x40) ? (CT_SD2| CT_BLOCK) : CT_SD2;
            	}
            }
        }
        else {
        	  test = send_cmd(ACMD41, 0x0);
        	  if(test <= 0x1){
        		  ty = CT_SD1; cmd = ACMD41;
        	  }
        	  else{
        		  ty = CT_MMC; cmd = CMD1;
        	  }
        	  while (Timer1 && send_cmd(CMD58, 0)) ;
        	  test = send_cmd(CMD16, 512);
        	  if ((!Timer1 || test) != 0)	/* Set block length: 512 */
        	      ty = 0;
        }


    }
    CardType = ty;	/* Card type */
    deselect_card();

    if(ty){
        /*Set fast frequency  */
        SPI1->CR1 &= ~ (SPI_CR1_BR);
        SPI1->CR1 |= SPI_CR1_BR_0;

    	Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
    		}
    else {			/* Failed */
    			Stat = STA_NOINIT;
    		}

    return Stat;
}



/////////////////////////////////////////////

DRESULT disk_read (
	BYTE drv,		/* Physical drive number (0) */
	BYTE *buff,		/* Pointer to the data buffer to store read data */
	LBA_t sector,	/* Start sector number (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DWORD sect = (DWORD)sector;


	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	if (!(CardType & CT_BLOCK)) sect *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector read */
	    BYTE test = send_cmd(CMD17, sect);
		if ((send_cmd(CMD17, sect) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512)) {
			count = 0;
		}
	}
	else {				/* Multiple sector read */
		if (send_cmd(CMD18, sect) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect_card();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}

/*-----------------------------------------------------------------------*/
/* Write sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
DRESULT disk_write (
    BYTE drv,           /* Physical drive number (0) */
    const BYTE *buff,   /* Ponter to the data to write */
    LBA_t sector,       /* Start sector number (LBA) */
    UINT count          /* Number of sectors to write (1..128) */
)
{
    DWORD sect = (DWORD)sector;


    if (drv || !count) return RES_PARERR;       /* Check parameter */
    if (Stat & STA_NOINIT) return RES_NOTRDY;   /* Check drive status */
    if (Stat & STA_PROTECT) return RES_WRPRT;   /* Check write protect */

    if (!(CardType & CT_BLOCK)) sect *= 512;    /* LBA ==> BA conversion (byte addressing cards) */

    if (count == 1) {   /* Single sector write */
        if ((send_cmd(CMD24, sect) == 0)    /* WRITE_BLOCK */
            && xmit_datablock(buff, 0xFE)) {
            count = 0;
        }
    }
    else {              /* Multiple sector write */
        if (CardType & CT_SDC) send_cmd(ACMD23, count); /* Predefine number of sectors */
        if (send_cmd(CMD25, sect) == 0) {   /* WRITE_MULTIPLE_BLOCK */
            do {
                if (!xmit_datablock(buff, 0xFC)) break;
                buff += 512;
            } while (--count);
            if (!xmit_datablock(0, 0xFD)) count = 1;    /* STOP_TRAN token */
        }
    }
    deselect_card();

    return count ? RES_ERROR : RES_OK;  /* Return result */
}
#endif

