#include "stm32f0xx.h"
#include "stdbool.h"
#define SECOND 1000

//global variable: Timer 1 and Timer 2
volatile uint16_t Timer1;
volatile uint16_t Timer2;

//initialize timer and interrupt
void init_tim6(volatile uint16_t Prescalar,volatile uint16_t Arr);

//initialize timer2
void init_tim2();


//disable timer
void disable_timer();
