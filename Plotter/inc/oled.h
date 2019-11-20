#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>

////////////////////////////
// function : output string to lcd with user offset
// input : str1 str2 offset1,offset2

//////////////////////////////
void lcd_output(const char *str1,const char *str2, 
int final_offset1,int final_offset2);

////////////////////////////
// function : initialize spi forf lcd


//////////////////////////////
void spi_init_lcd(void);