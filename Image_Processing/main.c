#include <stdio.h>
#include "bmp.h"

#define INPUT_FILE "input.bmp"
#define OUTPUT_FILE "output.bmp"

int main(int argc, char* argv[]) {
    BMPImage* image = read_bmp(INPUT_FILE);
    BMPImage* bwimg = black_white(image);
    write_bmp(bwimg, OUTPUT_FILE);
    free_bmp(image);
    free_bmp(bwimg);
    return 0;
}