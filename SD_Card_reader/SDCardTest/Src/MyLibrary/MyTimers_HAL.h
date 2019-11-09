/*
 * MyTimers.h
 *
 *  Created on: 28.04.2017
 *      Author: NELO
 */

#ifndef MYTIMERS_H_
#define MYTIMERS_H_

#include <stdint.h>

void InitandEnableDelayTimer15(void);
void delay_us(uint16_t delay_time);
void delay_ms(uint32_t delay_time);
void delay_s(uint32_t delay_time);


#endif /* MYTIMERS_H_ */

