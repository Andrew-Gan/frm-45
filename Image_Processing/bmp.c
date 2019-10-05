#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bmp.h"

#define PAPER_WIDTH 215.9

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
            
            if(mode == bw_rel || mode == grey_rel){    
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

/******************************************************************
 * void parse_bmp(BMPImage* img)
 * 
 * input:
 * img - black and white image to be converted to instruction code
 * auto_scaling - scale to paper size if true
******************************************************************/
void parse_bmp(BMPImage* img, bool auto_scaling) {
    int stack[2] = {0};
    int pixel_count = 0, size = img->header.image_size_bytes, bpp = img->header.bits_per_pixel / 8;
    bool isPenDown = false;
    unsigned char *data = img->data;
    int width_px = img->header.width_px, padding_byte = width_px % 4;
    float mm_per_pixel = PAPER_WIDTH / width_px;
    FILE* fp = fopen("instruction.txt", "w");
    for(int offs = 0; offs < size; offs += bpp, pixel_count++) {
        if(data[offs] > 128 && isPenDown || data[offs] <= 128 && !isPenDown) {
            isPenDown = !isPenDown;
            if(auto_scaling) {
                fprintf(fp, "%.1f %.1f %d\n", (pixel_count % width_px) * (mm_per_pixel), (pixel_count / width_px) * mm_per_pixel, isPenDown);
            }
            else {
                fprintf(fp, "%d %d %d\n", (pixel_count % width_px), (pixel_count / width_px), isPenDown);
            }
        }
        offs += (pixel_count + 1) % width_px == 0 ? padding_byte : 0;
        if((pixel_count + 1) % width_px == 0 && auto_scaling) {
            stack[0] = offs;
            stack[1] = pixel_count;
            // inner loop operation
            pixel_count -= width_px - 1;
            offs -= ((width_px - 1) * bpp) + padding_byte;
            for(float i = 0.1; i < mm_per_pixel; i += 0.1) {
                for(; pixel_count < width_px; offs += bpp, pixel_count++) {
                    if(data[offs] > 128 && isPenDown || data[offs] <= 128 && !isPenDown) {
                        isPenDown = !isPenDown;
                        fprintf(fp, "%.1f %.1f %d\n", (pixel_count % width_px) * mm_per_pixel, (pixel_count / width_px) * mm_per_pixel + i, isPenDown);
                    }
                }
                pixel_count -= width_px;
                offs -= width_px * bpp;
            }
            //end of inner loop operation
            offs = stack[0];
            pixel_count = stack[1];
        }
    }
    fclose(fp);
}