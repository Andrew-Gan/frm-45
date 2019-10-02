#include <stdio.h>

#include "bmpio.h"

BMPImage* read_bmp(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_SET);
    BMPImage* image = calloc(1, sizeof(*image));
    fread(&(image->header), 1, sizeof(BMPHeader), fp);
    image->data = calloc(1, image->header.image_size_bytes);
    fread(image->data, 1, image->header.image_size_bytes, fp);
    fclose(fp);
    return image;
}

void print_bmp(BMPImage* image, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    fwrite(&(image->header), 1, sizeof(BMPHeader), fp);
    fwrite(image->data, 1, image->header.image_size_bytes, fp);
    fclose(fp);
}

void close_bmp(BMPImage* image) {
    free(image->data);
    free(image);
}