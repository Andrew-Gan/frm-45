#include "stm32f0xx.h"
#include "stm32f0_discovery.h"





void init_exti(){
    //enable PA0, PA2, PC4-7 as EXTI
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC | SYSCFG_EXTICR2_EXTI5_PC | SYSCFG_EXTICR2_EXTI6_PC | SYSCFG_EXTICR2_EXTI7_PC;
    // rising edge trigger register for PC4-7
    EXTI->RTSR |= EXTI_RTSR_TR4 | EXTI_RTSR_TR5 | EXTI_RTSR_TR6 | EXTI_RTSR_TR7;
    // falling edge trigger register for PA0, PA2, PC4-7
    EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR2 | EXTI_FTSR_TR4 | EXTI_FTSR_TR5 | EXTI_FTSR_TR6 | EXTI_FTSR_TR7;
    // Interrupt mask register for PA0, PA2, PC4-7
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR2 | EXTI_IMR_MR4 | EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7;
    // NVIC trigger for EXTI0_1,EXTI2_3, EXTI4_15
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn | 1<<EXTI2_3_IRQn | 1<<EXTI4_15_IRQn;
    // Set prioriy for both Enter button should have higher priority
    NVIC_SetPriority(EXTI4_15_IRQn, 1);
    NVIC_SetPriority(EXTI0_1_IRQn,2);
    NVIC_SetPriority(EXTI2_3_IRQn,1);
}

