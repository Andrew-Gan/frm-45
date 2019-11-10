#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "cnclib.h"

#define STEP_SIZE 200*16/40

int this_x = 0, this_y = 0, last_x = 0, last_y = 0;

bool CW = true, CCW = false;
bool DIRECTION1, DIRECTION2;

void G0_cmd(float X, float Y){

    pen_up();
    move_to(X, Y);
}

void G1_cmd(float X, float Y){

    pen_down();
    move_to(X, Y);
}

void move_to(float X, float Y){

    int delta_y, delta_x, slope, ;
    this_x = round(X * STEP_SIZE);
    this_y = round(Y * STEP_SIZE);



}

