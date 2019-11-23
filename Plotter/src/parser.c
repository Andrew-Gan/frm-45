#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cnclib.h" // contains commands for G00 and G01

// length of buffer passed into parse_line()
#define BUFFER_LEN 128

// macro function for initialization of disp object
#define init_disp(disp) init_disp_field(disp.gcode); init_disp_field(disp.x); init_disp_field(disp.y); init_disp_field(disp.z);
#define init_disp_field(field) for(int i = 0; i < sizeof(field) / sizeof(*field); i++) {field[i] = '\0';}

int _spaceop(const char* buffer, int* start, int mode) {
    int tmp = *start;
    if(mode) {
        while(buffer[*start] == ' ' && buffer[*start] != '\n' && buffer[*start] != '\r' && buffer[*start] != '\0') {(*start)++;}
    }
    else {
        while(buffer[*start] != ' ' && buffer[*start] != '\n' && buffer[*start] != '\r' && buffer[*start] != '\0') {(*start)++;}
    }
    return *start - tmp;
}

// returns final position of pen based on mode (0:abs, 1:rel)
static Vector update_pos(int posMode, LCDdisp disp) {
    static Vector currPos = {.x = 0, .y = 0, .z = 0};
    Vector newDir = {
                        .x = posMode == 1 ? atoi(disp.x) : atoi(disp.x) - currPos.x,
                        .y = posMode == 1 ? atoi(disp.y) : atoi(disp.y) - currPos.y,
                        .z = posMode == 1 ? atoi(disp.z) : atoi(disp.z) - currPos.z
                    };
    currPos.x += newDir.x;
    currPos.y += newDir.y;
    currPos.z += newDir.z;
    return newDir;
}

// read lines in format: G0<code> <addr><value> <addr><value> ... \n
void parse_line(const char* buffer) {
    // posMode: 0-abs, 1-rel
    static int posMode = 0;
    // disp contain strings to be displayed on LED
    LCDdisp disp;
    init_disp(disp);
    if(buffer[0] == 'G') {
        memcpy(disp.gcode, &(buffer[0]), 3);
        int index = 4;
        // iterate through whole buffer until null terminator or end of line is found
        while(buffer[index] != '\0' && buffer[index] != '\n' && buffer[index] != '\r' && index < BUFFER_LEN) {
            _spaceop(buffer, &index, 1);
            char addr = buffer[index++];
            int tmp = index;
            int gap = _spaceop(buffer, &index, 0);
            switch(addr) {
                case 'X' : memcpy(disp.x, &(buffer[tmp]), gap);
                    break;
                case 'Y' : memcpy(disp.y, &(buffer[tmp]), gap);
                    break;
                case 'Z' : memcpy(disp.z, &(buffer[tmp]), gap);
            }
        }
        Vector newPos = update_pos(posMode, disp);
        // call corresponding step_control.c function
        if(buffer[1] == '0') {
            switch(buffer[2]) {
                case '0' : G0_cmd(newPos.y, newPos.x);
                    break;
                case '1' : G1_cmd(newPos.y, newPos.x);
                    break;
            }
        }
        else if(buffer[1] == '9') {
            switch(buffer[2]) {
                case '0' : posMode = 0;
                    break;
                case '1' : posMode = 1;
                    break;
            }
        }
    }
}
