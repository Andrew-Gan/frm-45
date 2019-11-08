
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "sd_card.h"
#include "oled.h"
#include "timer.h"

char button_pressed[] = "Pressed";
char nothing [] = "Button not pressed ";

void EXTI0_1_IRQHandler(){
       EXTI->PR |= EXTI_PR_PR0;
       Timer1 = 2* SECOND;
       lcd_output(button_pressed,0);

}

void init_exti(){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    EXTI-> FTSR |= EXTI_FTSR_TR0;
    EXTI-> IMR |= EXTI_IMR_MR0;
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn;
    NVIC_SetPriority(EXTI0_1_IRQn,1);
}

int main(void)
{
    FATFS fs;
    FATFS *pfs;
    DWORD fre_clust;
    uint32_t total, free_space;

    FRESULT fresult = f_mount(&fs, "", 0);
     fresult = f_getfree("", &fre_clust, &pfs);
//    BYTE test = disk_initialize(0);

    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    printf("%d",total);
    return 0;
}

    /*
     * This is to test EXTI Interrupt
     *
     */
//    volatile uint16_t prescalar = 480;
//    volatile uint16_t arr = 100;
//    init_tim6(prescalar, arr);
//
//    init_exti();
//    while(1){
//    Timer1 = 3* SECOND;
//    lcd_output(nothing,0);
//    }



    /* This is to test LCD output
//	char initialize[] = "    Successfully Initialize SD Card";
//		Timer1 = 5 * SECOND;
//		lcd_output(initialize,35);
//	char read[] = "     Reading SD Card...     ";
//	    Timer1 = 10 * SECOND;
//	    lcd_output(read,25);
    */

