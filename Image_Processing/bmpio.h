#ifndef __BMPIO_H__
#define __BMPIO_H__

#include "bmp.h"

BMPImage* read_img(const char*);
void print_BMP(BMPImage*, const char*);

#endif