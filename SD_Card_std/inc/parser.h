#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <math.h>

#define calcdist(x, y) sqrt(pow(x, 2) + pow(y, 2))

typedef struct {
    int x, y, z;
} Vector;

// struct containing strings to be displayed on LED
typedef struct {
    char gcode[4], x[10], y[10], z[10];
} LCDdisp;

void parse_line(const char*);

// mode 0 : skip until space char
// mode 1 : skip until non-space char
// return : amount of char skipped
int _spaceop(const char*, int*, int);

#endif
