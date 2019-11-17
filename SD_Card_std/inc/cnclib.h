/*
 * cnclib.h
 *
 *  Created on: Nov 5, 2019
 *      Author: mkrogh
 */

#ifndef INC_CNCLIB_H_
#define INC_CNCLIB_H_

extern int this_x, this_y, last_x, last_y;
volatile uint16_t Timer1;
volatile uint16_t Timer2;
typedef unsigned short  WORD;
void pen_up();
void pen_down();
void G0_cmd(int Y, int X);
void G1_cmd(int Y, int X);
void step_down();
void step_up();
void step_left();
void step_right();
void move_to(int Y, int X);


#endif /* INC_CNCLIB_H_ */
