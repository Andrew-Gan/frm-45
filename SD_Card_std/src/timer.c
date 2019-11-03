#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "timer.h"
#include "int.h"

void init_tim6(volatile uint16_t Prescalar,volatile uint16_t Arr) {
    // Your code goes here.
    //enable clock
       RCC->APB1ENR|=RCC_APB1ENR_TIM6EN;
    //    // trigger every 1ms
    //    TIM6->PSC = (volatile uint16_t)480-1;
    //    TIM6->ARR = (volatile uint16_t)100-1;
       TIM6->PSC = (volatile uint16_t)Prescalar-1;
       TIM6->ARR = (volatile uint16_t)Arr-1;
       
       // enable UIE
       TIM6->DIER |= TIM_DIER_UIE;
       NVIC->ISER[0] = 1<<TIM6_DAC_IRQn;
       TIM6->CR1 |= TIM_CR1_CEN;
}

void countdown_timer(){
    WORD n;
    n = Timer1;                     /* 1kHz decrement timer stopped at 0 */
    if (n) Timer1 = --n;
    n = Timer2;
    if (n) Timer2 = --n;
}

void TIM6_DAC_IRQHandler() {
    TIM6->SR &= ~TIM_SR_UIF;
    countdown_timer();
}
