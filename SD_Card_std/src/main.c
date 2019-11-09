#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "timer.h"
#include "ff.h"
#include "diskio.h"
#include "oled.h"
#include "button.h"

char nothing [] = "";


void clear_buffer(char *buff){
    for (int i= 0 ; i < 128 ; i++){
        buff[i] = '\0';
    }
}
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


int main(void)
{
    FATFS *fs;
    FATFS pfs;
    DWORD fre_clust, fre_sect, tot_sect;
    FIL test_file;
    FRESULT res;
    char buff1[128];
    char buff2[128];

    // Initialize the display.
	spi_init_lcd();
    // Initialize button
    init_exti();

    /* Get volume information and free clusters of drive 1 */
    res = f_mount(&pfs, "", 0);
    res  = f_getfree("", &fre_clust, &fs);



    //output to LCD if initialization succeed
    Timer1 = 1* SECOND;
    lcd_output("Mounting SD.",nothing,0);
    Timer1 = 1* SECOND;
    lcd_output("Mounting SD .",nothing,0);
    Timer1 = 1* SECOND;
    lcd_output("Mounting SD  .",nothing,0);


    if(res == FR_OK){
    Timer1 = 2* SECOND;
    lcd_output("Succeed!",nothing,0);
    }
    else{
        sprintf(buff1,"Error: %d", res);
        lcd_output(buff1,nothing,0);
        while(1);
    }

    /* Get total sectors and free sectors */
    tot_sect = ((fs->n_fatent - 2) * fs->csize) >> 2;
    fre_sect = (fre_clust * fs->csize ) >> 2;

    sprintf(buff1, "Total: %dMB", tot_sect);
    sprintf(buff2, "Free : %dMB", fre_sect);


    Timer1 = 5 * SECOND;
    lcd_output(buff1,buff2,0);
    clear_buffer(buff1);
    clear_buffer(buff2);

     res = f_open(&test_file, "test.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

    char test[] = "Fuck You\n";
    /* Writing text */
    res = f_puts(test,&test_file);

    /* Close file */
    res = f_close(&test_file);

    //
    return 0;
}
///*
// * This is to test EXTI Interrupt
// *
// */
//volatile uint16_t prescalar = 480;
//volatile uint16_t arr = 100;
//init_tim6(prescalar, arr);
//
//init_exti();
//while(1){
//Timer1 = 3* SECOND;
//lcd_output(nothing,0);
//}





    /* This is to test LCD output
//	char initialize[] = "    Successfully Initialize SD Card";
//		Timer1 = 5 * SECOND;
//		lcd_output(initialize,35);
//	char read[] = "     Reading SD Card...     ";
//	    Timer1 = 10 * SECOND;
//	    lcd_output(read,25);
    */

