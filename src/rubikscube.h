// header file for rubikscube.cc

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class EOFException {};

class Color {
private:
    /***************************************************************
    ** b: blue, g: green, y: yellow, r: red, o: orange, w: white  **
    ***************************************************************/
    char color;
public:
    Color();
    Color(char color);
    void setColor(char color);
    char getColor();
};

class CubePiece {
private:
    /***************************************************************
    ** Each CubePiece consists of maximum 3 colors as there are   **
    ** corners (3), edges (2) and surfaces (1). The color no-     **
    ** tation priority of each CubePiece is consistent:           **
    ** CORNERS -- (1) front-back, (2) top-bottom (3) left-right   **
    ** EDGES -- (1) front-back, (2) top-bottom (3) left-right     **
    ** SURFACES -- consist of only 1 color at [0]                 **
    ****************************************************************/
    vector<Color> colors;
public:
    CubePiece();
    CubePiece(string colorstring);
    void setColors(string colorstring);
    vector<Color> getColors();
    void print();
    bool isCornerPiece();
    bool isEdgePiece();
    bool isSurfacePiece();
};

class Cube {
private:
    //      [x: left-right] [y: front-back] [z: bottom-top]
    CubePiece cubePieces[3][3][3];
public:
    Cube();
    Cube(vector<CubePiece> pieces);
    void printFirstLayer();
    void printWholeCube();
    CubePiece setNewPiece(unsigned x, unsigned y, unsigned z, CubePiece cubePiece);
    void swapPieces(unsigned startX, unsigned startY, unsigned startZ, unsigned targetX, unsigned targetY, unsigned targetZ);
    void turn180Vert();
    Cube turnUp90Vert(Cube cube);
    Cube turnDown90Vert(Cube cube);
    Cube spin180(Cube cube);
    Cube spinRight90(Cube cube);
    Cube spinLeft90(Cube cube);
};



#endif
