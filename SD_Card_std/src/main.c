#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "timer.h"
#include "ff.h"
#include "diskio.h"
#include "oled.h"


char button_pressed[] = "Pressed";
char nothing [] = "Button not pressed ";


//FRESULT scan_files (
//    char* path        /* Start node to be scanned (***also used as work area***) */
//)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//    static FILINFO fno;
//
//
//    res = f_opendir(&dir, path);                       /* Open the directory */
//    if (res == FR_OK) {
//        for (;;) {
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                res = scan_files(path);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//
//            }
//        }
//        f_closedir(&dir);
//    }
//
//    return res;
//}

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
    FATFS *fs;
    FATFS pfs;
    DWORD fre_clust, fre_sect, tot_sect;
    FIL test_file;

    /* Get volume information and free clusters of drive 1 */
    FRESULT res = f_mount(&pfs, "", 0);
     res  = f_getfree("", &fre_clust, &fs);

    /* Get total sectors and free sectors */
    tot_sect = ((fs->n_fatent - 2) * fs->csize * 0.5);
    fre_sect = (fre_clust * fs->csize * 0.5);

    res = f_open(&test_file, "test.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    res = f_close(&test_file);
//   char test[] = "Fuck You\n";
//   /* Writing text */
//   res = f_puts(test,&fil);

   /* Close file */



    //
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

