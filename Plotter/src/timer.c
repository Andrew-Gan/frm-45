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

static void countdown_timer(){
    WORD n;
    n = Timer1;                     
    if (n) Timer1 = --n;
    n = Timer2;
    if (n) Timer2 = --n;
    
}

void TIM6_DAC_IRQHandler() {
    TIM6->SR &= ~TIM_SR_UIF;
    countdown_timer();
}

void disable_timer(){
    TIM6->CR1 &= ~TIM_CR1_CEN;
}


void init_tim2(){
    //Enable RCC GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    //Set PB3 as alternate function
    //clear it first
    GPIOB->MODER &= ~(3 << 2*3);
    //set it
    GPIOB->MODER |= (GPIO_Mode_AF << 2*3);

    //set pin for alternate function AF2
    GPIOB->AFR[0] |= (2 << 12) ;

    //enable timer 2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    //count up
    TIM2->CR1 &= ~TIM_CR1_DIR;

    //set it to 1.6kHz trigger
    TIM2->PSC = 48-1;
    TIM2->ARR = 625-1;

    // 2% duty cycle
    TIM2->CCR2 = 625 - 30;

    //channel 2 PWM mode 1
    //output is high as long as CNT < CCR2
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 |  TIM_CCMR1_OC2M_2 ;

    //enable output for TIM2
    TIM2->CCER |=  TIM_CCER_CC2E;

    //enable timer2

}
