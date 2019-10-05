#ifndef __BMP_H__
#define __BMP_H__

#include<stdint.h>

#pragma pack(1)

typedef struct {              // Total: 54 bytes
  uint16_t  signature;        // Magic identifier
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeader;

typedef struct {
    BMPHeader header;
    unsigned char* data;
} BMPImage;

#pragma pack()

/******************************************************************
 * typedef enum algoMode
 * 
 * bw_rel   - convert pixel to B/W by comparing with surrounding pixels
 * bw_abs   - convert pixel to B/W by comparing with threshold luminance
 * grey_rel - convert pixel to grey by setting to ratio of self to surrounding
 * grey_abs - convert pixel to grey by setting to average rbg of self
******************************************************************/
typedef enum {
    bw_rel = 0,
    bw_abs = 1,
    grey_rel = 2,
    grey_abs = 3,
    red_abs = 4
} algoMode;

BMPImage* read_bmp(const char*);
void write_bmp(BMPImage*, const char*);
void free_bmp(BMPImage*);
BMPImage* process_bmp(BMPImage*, algoMode, float);
void parse_bmp(BMPImage*, bool);

#endif