/*
 * cnclib.h
 *
 *  Created on: Nov 5, 2019
 *      Author: mkrogh
 */

#ifndef INC_CNCLIB_H_
#define INC_CNCLIB_H_

extern int this_x = 0, this_y = 0, last_x = 0, last_y = 0;

void pen_up();
void pen_down();
void G0_cmd(float X, float Y);
void G1_cmd(float X, float Y);


#endif /* INC_CNCLIB_H_ */
