#include "stm32f0xx.h"
#include "stm32f0_discovery.h"





void init_exti(){
    //enable PA0 as EXTI
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // falling edge trigger register for PA0
    EXTI-> FTSR |= EXTI_FTSR_TR0;
    // falling edge trigger register for PA2
    EXTI-> FTSR |= EXTI_FTSR_TR2;
    // Interrupt mask register for PA0
    EXTI-> IMR |= EXTI_IMR_MR0;
    // Interrupt mask register for PA2
    EXTI-> IMR |= EXTI_IMR_MR2;
    // NVIC trigger for EXTI0_1
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn;
    // NVIC trigger for EXTI2_3
    NVIC->ISER[0] = 1<<EXTI2_3_IRQn;
    // Set prioriy for both Enter button should have higher priority
    NVIC_SetPriority(EXTI0_1_IRQn,2);
    NVIC_SetPriority(EXTI2_3_IRQn,1);
}

