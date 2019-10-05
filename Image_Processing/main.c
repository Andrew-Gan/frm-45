#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"


#define INPUT_FILE "input.bmp"

int main(int argc, char* argv[]) {
    BMPImage* src_image = read_bmp(INPUT_FILE);
    BMPImage* des_image = process_bmp(src_image, bw_abs, 0.5);
    parse_bmp(des_image, true);
    write_bmp(des_image, "bw_abs.bmp");
    free_bmp(src_image);
    free_bmp(des_image);
    return 0;
}