#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>

#define up 20
#define down 1000

void tim1_init(int pen_mode) {
    /* Student code goes here */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0x2A0000;
    GPIOA->AFR[1] |= 0x222;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->PSC = 239;
    TIM1->ARR = 3999;
    TIM1->CCR1 = pen_mode;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1
     | TIM_CCMR1_OC1PE;
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_CEN;

}

void pen_up(){
    tim1_init(up);
}

void pen_down(){
    tim1_init(down);
}
