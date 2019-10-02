#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

// helper functions not to be called from outside
void _dupl_bmp(BMPImage*, BMPImage*);
int _cmp_px(unsigned int, unsigned int, int);

/******************************************************************
 * BMP Handling Functions
 * 
 * Public:
 * read_bmp: read from .bmp file and store as BMPImage
 * write_bmp: print content of BMPImage to .bmp file
 * free_bmp: free memory allocated to BMPImage struct type
 * 
 * Helper:
 * _dupl_bmp: duplicate from source to dest bmp, both malloc'ed
 * 
******************************************************************/
BMPImage* read_bmp(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    BMPImage* image = calloc(1, sizeof(*image));
    fread(&(image->header), 1, sizeof(image->header), fp);
    fseek(fp, sizeof(BMPHeader), SEEK_SET);
    image->data = calloc(1, image->header.image_size_bytes);
    fread(image->data, 1, image->header.image_size_bytes, fp);
    fclose(fp);
    return image;
}

void write_bmp(BMPImage* image, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    fwrite(&(image->header), 1, sizeof(BMPHeader), fp);
    fwrite(image->data, 1, image->header.image_size_bytes, fp);
    fclose(fp);
}

void free_bmp(BMPImage* image) {
    free(image->data);
    free(image);
}

void dupl_bmp(BMPImage* src_bmp, BMPImage* des_bmp) {
    des_bmp->header = src_bmp->header;
    memcpy(des_bmp->data, src_bmp->data, src_bmp->header.image_size_bytes);
}

/******************************************************************
 * BMP to Black and White Functions
 * 
 * Public:
 * black_white: convert bmp into black white image
 * 
 * Helper:
 * _cmp_px: compare pixel with surrounding
 * 
******************************************************************/
BMPImage* black_white(BMPImage* img) {
    BMPImage* bw_bmp = calloc(1, sizeof(*bw_bmp));
    bw_bmp->data = calloc(1, img->header.image_size_bytes);
    dupl_bmp(img, bw_bmp);
    int bytes_per_pixel = img->header.bits_per_pixel / 8;
    int width_px = img->header.width_px, height_px = img->header.height_px;
    int padding_byte = (width_px * bytes_per_pixel / 4 + 1) % 4;
    for(int row = 0; row < height_px; row++) {
        for(int centrePx = 0, surrPx = 0, px_count = 0, col = 0; col < width_px; col++) {
            unsigned int dataOffs = (row + padding_byte) * col;
            centrePx += img->data[dataOffs];
            centrePx += img->data[dataOffs + 1];
            centrePx += img->data[dataOffs + 2];
            if(row != 0) {
                // consider pixel above
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + 1];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + 2];
                px_count++;
            }
            if(row != width_px - 1) {
                // consider pixel below
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + 1];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + 2];
                px_count++;
            }
            if(col != 0) {
                // consider pixel to the left
                surrPx += img->data[dataOffs - bytes_per_pixel];
                surrPx += img->data[dataOffs - bytes_per_pixel + 1];
                surrPx += img->data[dataOffs - bytes_per_pixel + 2];
                px_count++;
            }
            if(col != width_px - 1) {
                // consider pixel to the right
                surrPx += img->data[dataOffs + bytes_per_pixel];
                surrPx += img->data[dataOffs + bytes_per_pixel + 1];
                surrPx += img->data[dataOffs + bytes_per_pixel + 2];
                px_count++;
            }
            if(row != 0 && col != 0) {
                // consider pixel to the top left
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel + 1];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel + 2];
                px_count++;
            }
            if(row != 0 && col != width_px - 1) {
                // consider pixel to the top right
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel + 1];
                surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel + 2];
                px_count++;
            }
            if(row != height_px - 1 && col != 0) {
                // consider pixel to the bottom left
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel + 1];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel + 2];
                px_count++;
            }
            if(row != height_px - 1 && col != width_px - 1) {
                // consider pixel to the bottom right
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel + 1];
                surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel + 2];
                px_count++;
            }
            int ret = _cmp_px(centrePx, surrPx, px_count);
            // modify duplicated bmp file
            bw_bmp->data[dataOffs] = ret;
            bw_bmp->data[dataOffs + 1] = ret;
            bw_bmp->data[dataOffs + 2] = ret;
        }
    }
    return bw_bmp;
}

int _cmp_px(unsigned int centrePx, unsigned int surrPx, int px_count) {
    centrePx *= px_count;
    int ret = centrePx < surrPx ? 0 : 255;
    return ret;
}