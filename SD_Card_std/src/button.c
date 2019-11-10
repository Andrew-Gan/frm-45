#include "stm32f0xx.h"
#include "stm32f0_discovery.h"





void init_exti(){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    EXTI-> FTSR |= EXTI_FTSR_TR0;
    EXTI-> IMR |= EXTI_IMR_MR0;
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn;
    NVIC_SetPriority(EXTI0_1_IRQn,1);
}

