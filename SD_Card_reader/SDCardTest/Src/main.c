/*
 * 	MAIN
 *	TEST FOR SD CARD
 *  Created on: 09.05.2018
 *      Author: NELO
 */

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "SystemConfig.h"
#include "MyTimers_HAL.h"
#include "MySPI_HAL.h"
#include "diskio.h"
#include "ff.h"
#include "ffconf.h"
#include "integer.h"
#include "SDCard.h"

void failed(void){
	////Do something here
	////Or set a breakpoint
	delay_s(10);
}

int main(void)
{
	BYTE TxBuff[512];					////Tx Data Buffer
	BYTE RxBuff[512];					////Rx Data Buffer
	uint32_t i = 0;						////Counter
	char res = 1;						////Variable storing response
	char newline[2] = {0x0D, 0x0A};		////New line codes (ASCII Table)
	char tab[2] = {0x09, 0x09};			////2 x TAB code (ASCII Table)
	char snum[5] = "";					////Array for write iteration
	FATFS fs;							////FATFS object
	FIL file;							////FILE object

	SystemClock_Config();				////System Configuration
	InitandEnableDelayTimer15();		////Init Timer for delays

	for(i=0;i<512;i++){					////Initialize Data Buffers
		TxBuff[i] = 0x44;
		RxBuff[i] = 0x00;
	}

	////1) Test if disk_initialize() functions works!
	res =  disk_initialize(0);
	if(res != FR_OK){
		failed();
		return 1;		////FAILED!
	}
	delay_s(1);

	////2) Test if disk_write(..) functions works!
	////NOTE: Check also if data is correct (with HxD for example) and not only the res variable!
	res =  disk_write (
			0,			/* Physical drive number (0) */
			TxBuff,		/* Pointer to the data to write */
			44444,		/* Start sector number (LBA) */
			1			/* Number of sectors to write (1..128) */
		);
	if(res != FR_OK){
		failed();
		return 1;		////FAILED!
	}
	delay_s(1);

	////3) Test if disk_read(..) functions works!
	////NOTE: Check also if data is correct (with debugger) and not only the res variable!
	res =  disk_read (
			0,			/* Physical drive number (0) */
			RxBuff,		/* Pointer to the data buffer to store read data */
			44444,		/* Start sector number (LBA) */
			1			/* Number of sectors to read (1..128) */
		);
	if(res != FR_OK){
		failed();
		return 1;		////FAILED!
	}
	delay_s(1);

	////4) Test if f_mount(..) works!
	res =  f_mount(&fs, "0", 1);
	if(res != FR_OK){
		failed();
		return 1;		////FAILED!
	}
	delay_s(1);

	////5) Test if f_open(..) works!!
	res =  f_open(&file, "TESTFILE.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ);
	if(res != FR_OK){
		failed();
		return 1;		////FAILED!
	}
	delay_s(1);

	if (res == FR_OK){
		////Write 100 Lines to Text File
		for(i=1;i<=100;i++){
			res =  f_open(&file, "TESTFILE.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ);
			if (res == FR_OK){
				itoa(i,snum,10);
				res = f_write(&file, snum, 5, NULL);
				if(res != FR_OK){
					f_close(&file);
					failed();
					return 1;		////FAILED!
				}
				res = f_write(&file, tab, 2, NULL);
				if(res != FR_OK){
					f_close(&file);
					failed();
					return 1;		////FAILED!
				}
				res = f_write(&file, "Saving some text: XX.YYY.ZZZ ; 1.22.333.4444.55555 !!", 53, NULL);
				if(res != FR_OK){
					f_close(&file);
					failed();
					return 1;		////FAILED!
				}
				res = f_write(&file, newline, 2, NULL);
				if(res != FR_OK){
					f_close(&file);
					failed();
					return 1;		////FAILED!
				}
				f_close(&file);
				delay_ms(100);
			}else{
				f_close(&file);
				failed();
				return 1;
			}
		}
	}else{
		failed();
		return 1;
	}

	return 0;		////SUCCEEDED!
}





