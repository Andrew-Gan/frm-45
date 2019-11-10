#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <vector>
#include <math.h>

#define calcdist(x, y) sqrt(pow(x, 2) + pow(y, 2))

using namespace std;

struct Vector {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

class GCode {
private:
    int code;
    Vector coords;
    Vector arc;
    float feedrate = 0.0;
public:
    GCode(int, Vector, Vector, float); // float[] is length 6, 0:x, 1:y, 2:z, 3:i, 4:j, 5:f
    virtual ~GCode() {};
    int getCode() {return code;}
    Vector getCoords() {return coords;}
    Vector getArc() {return arc;}
    float getFeedrate() {return feedrate;}
    vector<GCode> curve_to_lin(float[], int);
};

GCode::GCode(int mycode, Vector coordArg, Vector arcArg, float feedArg) {
    code = mycode;
    coords = coordArg;
    arc = arcArg;
    feedrate = feedArg;
}

vector<GCode> GCode::curve_to_lin(float currPos[], int res) {
    if(code != 2 && code != 3) {
        cout << "Invalid call to curve_to_lin(). GCode is not G02 or G03." << endl;
    }
    if(calcdist(arc.x, arc.y) != calcdist(coords.x - (currPos[0] + arc.x), coords.y - (currPos[1] + arc.y))) {
        cout << "Error in curve_to_lin(). Two endpoints not equidistant to arc center." << endl;
    }
    vector<GCode> segments;
    int circum = 2 * M_PI * calcdist(arc.x, arc.y);
    // not done
    return segments;
}

// mode 0 : skip until space char
// mode 1 : skip until non-space char
// return : amount of char skipped
int _spaceop(string str, int& start, int mode) {
    int tmp = start;
    if(mode) {
        while(str.at(start) == ' ' && str.at(start) != '\n' && str.at(start) != '\r') {start++; if(start >= str.size()){break;}}
    }
    else {
        while(str.at(start) != ' ' && str.at(start) != '\n' && str.at(start) != '\r') {start++; if(start >= str.size()){break;}}
    }
    return start - tmp;
}

#endif