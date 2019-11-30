#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "cnclib.h"
#include "timer.h"

#define STEP_SIZE 2
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int this_x = 0, this_y = 0, last_x = 0, last_y = 0;

bool CW = true, CCW = false;
bool DIRECTION1, DIRECTION2;



void G0_cmd(int Y, int X){
    float x = X, y = Y;
    Timer1 = 50;
    pen_up();
    while(Timer1){}
    move_to(x, y);
}

void G1_cmd(int Y, int X){
    Timer1 = 50;
    pen_down();
    while(Timer1){}
    move_to(X, Y);
}
void stepper_move(){


    if(DIRECTION1 == CW){
        GPIOB->ODR |= 1 << 7;
    }else{
        GPIOB->ODR &= ~1 << 7;
    }
    if(DIRECTION2 == CW){
        GPIOB->ODR |= 1 << 5;
    }else{
        GPIOB->ODR &= ~1 << 5;
    }


    GPIOB->BSRR = GPIO_BSRR_BR_6;




    TIM2->CR1 |= TIM_CR1_CEN;

    Timer1=30;
    while(Timer1){};

    TIM2->CR1 &= ~TIM_CR1_CEN;
    GPIOB->BSRR = GPIO_BSRR_BS_6;


}

void step_down(){
    DIRECTION1 = CW;
    DIRECTION2 = CCW;
    stepper_move();
}

void step_up(){
    DIRECTION1 = CCW;
    DIRECTION2 = CW;
    stepper_move();
}

void step_right(){
    DIRECTION1 = CW;
    DIRECTION2 = CW;
    stepper_move();
}

void step_left(){
    DIRECTION1 = CCW;
    DIRECTION2 = CCW;
    stepper_move();
}

void move_to(int X, int Y){

    int delta_y, delta_x, temp_x, temp_y;
    int slope=0, longest=0, shortest=0, maximum=0, error=0, threshold=0;

    temp_x = last_x;
    temp_y = last_y;
    this_x = X * STEP_SIZE;
    this_y = Y * STEP_SIZE;

    delta_x = this_x - temp_x;
    delta_y = this_y - temp_y;

    longest = MAX(abs(delta_y), abs(delta_x));
    shortest = MIN(abs(delta_y), abs(delta_x));

    error = -longest;                     //add offset to so we can test at zero
    threshold = 0;                        //test now done at zero
    maximum = (longest << 1);             //multiply by two
    slope = (shortest << 1);              //multiply by two ... slope equals (shortest*2/longest*2)

    bool axis_swap = true;
    if(abs(delta_x) >= abs(delta_y)){
        axis_swap = false;
    }

    for(int i = 0; i < longest; i++){

        if(axis_swap){
            if(delta_y < 0){
                temp_y--;
                step_down();
            }else{
                temp_y++;
                step_up();
            }
        }else{
            if(delta_x < 0){
                temp_x--;
                step_left();
            }else{
                temp_x++;
                step_right();
            }
        }

        error += slope;
            if (error > threshold) {
                error -= maximum;

                if(axis_swap){
                    if(delta_x < 0){
                        temp_x--;
                        step_left();
                    }else{
                        temp_x++;
                        step_right();
                    }
                }else{
                    if(delta_y < 0){
                        temp_y--;
                        step_down();
                    }else{
                        temp_y++;
                        step_up();
                    }
                }
            }

    }




}

