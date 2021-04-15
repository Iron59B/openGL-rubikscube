// header file for rubikscube.cc

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class EOFException {};

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

    vector<char> colors;
public:
    CubePiece();
    CubePiece(string colorstring);
    void setColors(string colorstring);
    vector<char> getColors();
    void print();
    bool isCornerPiece();
    bool isEdgePiece();
    bool isSurfacePiece();
};

class Cube {
private:
    //      [x: left-right] [y: front-back] [z: bottom-top]
    CubePiece cubes[3][3][3];
public:
    Cube();
    Cube(vector<CubePiece> cubePieces);
    void printFirstLayer();
    CubePiece setNewPiece(unsigned x, unsigned y, unsigned z, CubePiece cubePiece);
};

Cube swapCubePiecesDiagonal(Cube cube, unsigned initX, unsigned initY, unsigned initZ, unsigned targetX, unsigned targetY, unsigned targetZ);
Cube turn180Vert(Cube cube);
Cube turnUp90Vert(Cube cube);
Cube turnDown90Vert(Cube cube);
Cube spin180(Cube cube);
Cube spinRight90(Cube cube);
Cube spinLeft90(Cube cube);

#endif
