#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bmp.h"

// helper functions not to be called from outside
void __dupl_bmp(BMPImage*, BMPImage*);

/******************************************************************
 * BMPImage* read_bmp(const char* filename)
 * 
 * input:
 * filename - name of file to read image content
 * 
 * output:
 * BMPImage* - content read from file, malloc'ed
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

/******************************************************************
 * void write_bmp(BMPImage* image, const char* filename)
 * 
 * input:
 * image - BMPImage* to be written to file
 * filename - name of file to write content
******************************************************************/
void write_bmp(BMPImage* image, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    fwrite(&(image->header), 1, sizeof(BMPHeader), fp);
    fwrite(image->data, 1, image->header.image_size_bytes, fp);
    fclose(fp);
}

/******************************************************************
 * void free_bmp(BMPImage* image)
 * 
 * input:
 * image - BMPImage* to be deallocated
******************************************************************/
void free_bmp(BMPImage* image) {
    free(image->data);
    free(image);
}

void _dupl_bmp(BMPImage* src_bmp, BMPImage* des_bmp) {
    des_bmp->header = src_bmp->header;
    memcpy(des_bmp->data, src_bmp->data, src_bmp->header.image_size_bytes);
}

/******************************************************************
 * BMPImage* process_bmp(BMPImage* img, algoMode mode, float thres_val)
 * 
 * input:
 * img - source image to be processed
 * mode - algorithm 'algoMode' to be applied
 * thres_val - threshold value for pixel colour
 * 
 * output:
 * BMPImage* - malloc'ed processed image file
******************************************************************/
BMPImage* process_bmp(BMPImage* img, algoMode mode, float thres_val) {
    BMPImage* bw_bmp = calloc(1, sizeof(*bw_bmp));
    bw_bmp->data = calloc(1, img->header.image_size_bytes);
    _dupl_bmp(img, bw_bmp);
    int bytes_per_pixel = img->header.bits_per_pixel / 8;
    int width_px = img->header.width_px, height_px = img->header.height_px;
    int padding_byte = width_px  % 4, dataOffs, ret;
    // row and col starts from bottom left towards top right
    for(int row = 0; row < height_px; row++) {
        for(int col = 0; col < width_px; col++) {
            int centrePx = 0, surrPx = 0, px_count = 0;
            dataOffs = row * (width_px * bytes_per_pixel + padding_byte) + (col * bytes_per_pixel);
            centrePx += img->data[dataOffs];
            centrePx += img->data[dataOffs + 1];
            centrePx += img->data[dataOffs + 2];
            
            if(mode == bw_rel || mode == bw_abs){    
                if(row != width_px - 1) {
                    // consider pixel above
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + 1];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + 2];
                    px_count++;
                }
                if(row != 0) {
                    // consider pixel below
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + 1];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + 2];
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
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel + 1];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte - bytes_per_pixel + 2];
                    px_count++;
                }
                if(row != height_px - 1 && col != width_px - 1) {
                    // consider pixel to the top right
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel + 1];
                    surrPx += img->data[dataOffs + (width_px * bytes_per_pixel) + padding_byte + bytes_per_pixel + 2];
                    px_count++;
                }
                if(row != 0 && col != 0) {
                    // consider pixel to the bottom left
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel + 1];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte - bytes_per_pixel + 2];
                    px_count++;
                }
                if(row != 0 && col != width_px - 1) {
                    // consider pixel to the bottom right
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel + 1];
                    surrPx += img->data[dataOffs - (width_px * bytes_per_pixel) - padding_byte + bytes_per_pixel + 2];
                    px_count++;
                }
                centrePx *= px_count;
                ret = mode == bw_rel ? centrePx <= surrPx ? 0 : 255 : (int)((float)centrePx / surrPx * (thres_val * 255));
                bw_bmp->data[dataOffs] = ret;
                bw_bmp->data[dataOffs + 1] = ret;
                bw_bmp->data[dataOffs + 2] = ret;
            }

            else if (mode == bw_abs) {
                bw_bmp->data[dataOffs] = centrePx > ((thres_val * 255) * 3) ? 255 : 0;
                bw_bmp->data[dataOffs + 1] = centrePx > ((thres_val * 255) * 3) ? 255 : 0;
                bw_bmp->data[dataOffs + 2] = centrePx > ((thres_val * 255) * 3) ? 255 : 0;
            }

            else {
                bw_bmp->data[dataOffs] = (int)((float)centrePx / 3);
                bw_bmp->data[dataOffs + 1] = (int)((float)centrePx / 3);
                bw_bmp->data[dataOffs + 2] = (int)((float)centrePx / 3);
            }
        }
    }
    return bw_bmp;
}