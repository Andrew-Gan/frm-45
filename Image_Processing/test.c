#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"

#define INPUT_FILE "input.bmp"
#define OUTPUT_FILE "output.bmp"

int main(int argc, char* argv[]) {
    BMPImage* src_image = read_bmp(INPUT_FILE);
    BMPImage* des_image = process_bmp(src_image, grey_rel, 0.7);
    write_bmp(des_image, OUTPUT_FILE);
    free_bmp(src_image);
    free_bmp(des_image);
    return 0;
}