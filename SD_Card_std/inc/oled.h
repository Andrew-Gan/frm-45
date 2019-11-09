#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>


void lcd_output(const char *str1,const char *str2, int final_offset);
void spi_init_lcd(void);