
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "sd_card.h"
#include "oled.h"


int main(void)
{


	DSTATUS Stat = SD_initialize();


	if (Stat == STA_NOINIT){

	}
	if(Stat ==STA_NODISK){

	}
	if(Stat == STA_PROTECT ){

	}
	char str[] = "  Successfully Initialize SD      ";
		lcd_output(str);


}
