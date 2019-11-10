#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <stdio.h>
#include "my_spi.h"
#include "timer.h"
#include "oled.h"

int offset1 = 0;
int offset2 = 0;

//=========================================================================
// An inline assembly language version of nano_wait.
//=========================================================================
static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

static void spi_cmd(char b) {
    // Your code goes here.
    while ( (SPI2->SR & SPI_SR_TXE) == 0 );
    SPI2->DR = b;

}

static void generic_lcd_startup(void) {
    nano_wait(100000000); // Give it 100ms to initialize
    spi_cmd(0x38);  // 0011 NF00 N=1, F=0: two lines
    spi_cmd(0x0c);  // 0000 1DCB: display on, no cursor, no blink
    spi_cmd(0x01);  // clear entire display
    nano_wait(6200000); // clear takes 6.2ms to complete
    spi_cmd(0x02);  // put the cursor in the home position
    spi_cmd(0x06);  // 0000 01IS: set display to increment
    spi_cmd(0x80 + 0); // put the cursor on the beginning of the first line (offset 0).
}



static void spi_data(char b) {
    // Your code goes here.
    while ( (SPI2->SR & SPI_SR_TXE) == 0 );
    SPI2->DR = 0x200 + b;
}

//configures the appropriate I/O pins so that they are
//routed to SPI channel 2.  It should also initialize SPI channel 2 so that it is configured for bidirectional
//mode, with bidirectional output enabled, currently set as master, and the slowest possible baud rate.
//Configure the clock to be 0 when idle, and use the first clock transition as the data capture edge.  Also
//configure it to use a 10-bit word size, slave select output enable, and automatic NSS pulse generation.
//Enable the SPI2 channel by setting the SPE bit.  Finally, this subroutine should invoke the initialization
//procedures for the LCD by calling
//generic_lcd_startup()
//.
void spi_init_lcd(void) {
    // Your code goes here.

    //enable clock for SPI and GPIOB
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    //clear RCC MODER
    GPIOB->MODER &= ~(3 << (2*12));
    GPIOB->MODER &= ~(3 << (2*13));
    GPIOB->MODER &= ~(3 << (2*15));
    // set RCC MODER
    GPIOB->MODER |= GPIO_Mode_AF << (2*12);
    GPIOB->MODER |= GPIO_Mode_AF << (2*13);
    GPIOB->MODER |= GPIO_Mode_AF << (2*15);
    // bidirectional mode, bidirectional enable
    SPI2->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;
    //master configuration
    SPI2->CR1 |= SPI_CR1_MSTR;
    //slowest baud rate
    SPI2->CR1 |= SPI_CR1_BR_2| SPI_CR1_BR_0 | SPI_CR1_BR_1;
    //clock to 0 when idle
    SPI2->CR1 &= ~SPI_CR1_CPOL;
    //first clock transition as the data capture edge
    SPI2->CR1 &= ~SPI_CR1_CPHA;
    //use a 10-bit word size
    SPI2->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0;
    //slave select output enable
    SPI2->CR2 |= SPI_CR2_SSOE;
    //automatic NSS pulse generation
    SPI2->CR2 |= SPI_CR2_NSSP;
    // Enable the SPI2 channel by setting the SPE bit
    SPI2->CR1 |= SPI_CR1_SPE;

    generic_lcd_startup();


}

static void nondma_display1(const char *s) {

    spi_cmd(0x80 + 0); // put the cursor on the beginning of the first line (offset 0).
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
        	spi_data(s[x]);
        else
            break;
    for(   ; x<16; x+=1)
    	spi_data(' ');
}

static void nondma_display2(const char *s) {
    // put the cursor on the beginning of the second line (offset 64).
    spi_cmd(0x80 + 64);
    int x;
    for(x=0; x<16; x+=1)
        if (s[x] != '\0')
            spi_data(s[x]);
        else
            break;
    for(   ; x<16; x+=1)
        spi_data(' ');
}

void lcd_output(const char *str1,const char *str2, 
int final_offset1, int final_offset2) {

    // Write text.

    while(Timer1) {
        if(offset1 > final_offset1){
            offset1 = 0;
        }
        if(offset2 > final_offset2){
            offset2 = 0;
        }
    	nondma_display1(&str1[offset1]);
        nondma_display2(&str2[offset2]);
    	offset1++;
        offset2++;
    	Timer2 = 1000;
    	while(Timer2);
    }
}

