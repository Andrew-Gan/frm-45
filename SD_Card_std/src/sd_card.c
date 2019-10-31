
#include "stm32f0xx.h"
#include "diskio.h"
#include "sd_card.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}



void init_spi(){

}
