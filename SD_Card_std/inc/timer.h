#include "stm32f0xx.h"

//global variable: Timer 1 and Timer 2
volatile uint16_t Timer1;
volatile uint16_t Timer2;

//static countdown function
//will be called by Timer6 Interrupt
static void countdown_timer();

//initialize timer and interrupt
void init_tim6(volatile uint16_t Prescalar,volatile uint16_t Arr);

//disable timer
void disable_timer();