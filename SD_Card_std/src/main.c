
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "sd_card.h"
#include "oled.h"
#include "timer.h"


int main(void)
{


	DSTATUS Stat = SD_initialize();


	if (Stat == STA_NOINIT){

	}
	if(Stat ==STA_NODISK){

	}
	if(Stat == STA_PROTECT ){

	}
	char initialize[] = "    Successfully Initialize SD Card";
		Timer1 = 5 * SECOND;
		lcd_output(initialize,35);
	char read[] = "     Reading SD Card...     ";
	    Timer1 = 10 * SECOND;
	    lcd_output(read,25);


}
