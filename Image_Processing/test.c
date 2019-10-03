#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"

#define INPUT_FILE "input.bmp"

int main(int argc, char* argv[]) {
    BMPImage* src_image = read_bmp(INPUT_FILE);
    BMPImage* des_image_1 = process_bmp(src_image, bw_abs, 0.5);
    write_bmp(des_image_1, "bw_abs.bmp");
    BMPImage* des_image_2 = process_bmp(src_image, bw_rel, 0.5);
    write_bmp(des_image_2, "bw_rel.bmp");
    BMPImage* des_image_3 = process_bmp(src_image, grey_abs, 0.5);
    write_bmp(des_image_3, "grey_abs.bmp");
    BMPImage* des_image_4 = process_bmp(src_image, grey_rel, 0.5);
    write_bmp(des_image_4, "grey_rel.bmp");
    free_bmp(src_image);
    free_bmp(des_image_1);
    free_bmp(des_image_2);
    free_bmp(des_image_3);
    free_bmp(des_image_4);
    return 0;
}