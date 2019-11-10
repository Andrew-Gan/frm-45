#include <fstream>
#include <string>
#include "parser.hpp"

#define INPUT_FILE "Alien Face.nc"
#define OUTPUT_FILE "output.txt"
#define DEBUG_FILE "debug.txt"

// user config
#define CURVE_RES 5

using namespace std;

static const Vector vectorzero;

// read lines in format: G0<code> <addr><value> <addr><value> ... \n
vector<GCode> read_file(const char* filename) {
    vector<GCode> GCodeFile;
    ifstream fp(filename);
    int index = 4;
    if(fp.fail()) {return GCodeFile;}
    string currLine;
    Vector coords, arc;
    float feedrate = 0.0;
    do{
        getline(fp, currLine);
        if(!currLine.empty() && currLine.at(0) == 'G') {
            coords = vectorzero;
            arc = vectorzero;
            feedrate = 0.0;
            while(index < currLine.length() - 1) {
                _spaceop(currLine, index, 1);
                char addr = currLine.at(index++);
                int tmp = index;
                float val = atof(currLine.substr(tmp, _spaceop(currLine, index, 0)).c_str());
                switch(addr) {
                    case 'X' : coords.x = val;
                        break;
                    case 'Y' : coords.y = val;
                        break;
                    case 'Z' : coords.z = val;
                        break;
                    case 'I' : arc.x = val;
                        break;
                    case 'J' : arc.y = val;
                        break;
                    case 'F' : feedrate = val;
                }
            }
            GCodeFile.push_back(GCode(atoi(currLine.substr(1, 2).c_str()), coords, arc, feedrate));
        }
        index = 4;
    }while(!fp.eof());
    fp.close();
    return GCodeFile;
}

static void update_writePos(ofstream& fp, int posMode, Vector point) {
    static float xPos = 0, yPos = 0, zPos = 0;
    xPos = posMode ? xPos + point.x : point.x;
    yPos = posMode ? yPos + point.y : point.y;
    zPos = posMode ? zPos + point.z : point.z;
    fp << xPos << " " << yPos << " " << zPos << endl;
}

void process_pos(vector<GCode> GCodeFile, const char* filename) {
    ofstream ofp(filename);
    if(ofp.fail()) {return;}
    int posMode = 0; // 0:abs, 1:rel
    for(int i = 0; i < GCodeFile.size(); i++) {
        switch(GCodeFile[i].getCode()) {
            case 0 : update_writePos(ofp, posMode, GCodeFile[i].getCoords());
                break;
            case 1 : update_writePos(ofp, posMode, GCodeFile[i].getCoords());
                break;
            case 2 : update_writePos(ofp, posMode, GCodeFile[i].getCoords());
                break;
            case 3 : update_writePos(ofp, posMode, GCodeFile[i].getCoords());
                break;
            case 90 : posMode = 0;
                break;
            case 91 : posMode = 1;
                break;
            default : cout << "Unregistered G-code" << endl;
        }
    }
    ofp.close();
}

void debug_content(vector<GCode> GCodeFile) {
    ofstream fp(DEBUG_FILE);
    if(fp.fail()) {return;}
    for(int i = 0; i < GCodeFile.size(); i++) {
        fp << "G" << GCodeFile[i].getCode() << " ";
        fp << GCodeFile[i].getCoords().x << " " << GCodeFile[i].getCoords().y << " " << GCodeFile[i].getCoords().z << " ";
        fp << GCodeFile[i].getArc().x << " " << GCodeFile[i].getArc().y << endl;
        fp << GCodeFile[i].getFeedrate() << endl;
    }
    fp.close();
}

int main() {
    vector<GCode> GCodeFile = read_file(INPUT_FILE);
    process_pos(GCodeFile, OUTPUT_FILE);
    //debug_content(GCodeFile);
    return EXIT_SUCCESS;
}