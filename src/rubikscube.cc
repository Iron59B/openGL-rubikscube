/* standard includes */
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include "rubikscube.h"

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
#include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include<GLFW/glfw3.h>

using namespace std;


/**********************************************************************
**                      CLASS MEMBER DEFINITIONS                     **
**********************************************************************/

Color::Color(){}

Color::Color(char color) {
    this->color = color;
}

char Color::getColor() {
    return color;
}

void Color::setColor(char color) {
    this->color = color;
}

///////////////////////////////////////////////////////////////////////////////

CubePiece::CubePiece(){}

CubePiece::CubePiece(string colorstring) {
    unsigned i;
    for (i = 0; i < colorstring.size(); i++) {
        colors.push_back(Color(colorstring.at(i)));
    }
}

void CubePiece::setColors(string colorstring) {
    colors.clear();
    unsigned i;
    for (i = 0; i < colorstring.size(); i++) {
        colors.push_back(Color(colorstring.at(i)));
    }
}

vector<Color> CubePiece::getColors() {
    return colors;
}

void CubePiece::print() {
    string out = "";
    unsigned i;
    for (i = 0; i < colors.size(); i++) {
        if (i == colors.size() -1) {
            cout << out << colors[i].getColor() << "\n";
            return;
        }
        out = out + colors[i].getColor() + ", ";
    }
}

bool CubePiece::isCornerPiece() {
    if (colors.size() == 3)
        return true;
    else return false;
}
bool CubePiece::isEdgePiece() {
    if (colors.size() == 2)
        return true;
    else return false;
}
bool CubePiece::isSurfacePiece() {
    if (colors.size() == 1)
        return true;
    else return false;
}

// changes the color orientation of the cubepiece so it ist correctly oriented after the move
void CubePiece::prepareEdgePieceMoveRight90() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColor();
    char initCol2 = getColors().at(1).getColor();
    prepared = prepared + initCol2 + initCol1;
    setColors(prepared);
}

// changes the color orientation of the cubepiece so it ist correctly oriented after the move
void CubePiece::prepareCornerPieceMoveRight90() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColor();
    char initCol2 = getColors().at(1).getColor();
    char initCol3 = getColors().at(2).getColor();
    prepared = prepared + initCol3 + initCol2 + initCol1;
    setColors(prepared);
}


///////////////////////////////////////////////////////////////////////////////


Cube::Cube(){}

Cube::Cube(vector<CubePiece> pieces) {
    unsigned i, x, y, z;
    i = 0;

    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (i < 27) {
                    cubePieces[x][y][z] = pieces[i];
                    i++;
                }
            }
        }
    }
}

// prints first (bottom) layer
void Cube::printFirstLayer() {
    unsigned i, p;
    string out = "";

    for (i = 0; i < 3; i++) {
        for (p = 0; p < 3; p++) {
            cubePieces[p][i][0].print();
            cout << "\n";
        }
    }
}

void Cube::printWholeCube() {
    unsigned i, p, k;
    string out = "";

    for (i = 0; i < 3; i++) {
        if (i == 0)
            cout << "First layer: \n";
        else if (i == 1)
            cout << "Second layer: \n";
        else
            cout << "Third layer: \n";
        for (p = 0; p < 3; p++) {
            for (k = 0; k < 3; k++) {
                cubePieces[k][p][i].print();
                cout << "\n";
            }
        }
    }
}
// function writes new cubePiece on cubePieces[x][y][z] and returns old one
CubePiece Cube::setNewPiece(unsigned x, unsigned y, unsigned z, CubePiece cubePiece) {
    CubePiece tmp = cubePieces[x][y][z];
    cubePieces[x][y][z] = cubePiece;
    return tmp;
}

/**********************************************************************
**                  CUBE MOVEMENT FUNCTION DEFINITIONS               **
**********************************************************************/

// swaps CubePiece[startX][startY][startZ] with CubePiece[targetX][targetY][targetZ]
void Cube::swapPieces(unsigned startX, unsigned startY, unsigned startZ, unsigned targetX, unsigned targetY, unsigned targetZ) {
    CubePiece tmp = setNewPiece(targetX, targetY, targetZ, cubePieces[startX][startY][startZ]);
    setNewPiece(startX, startY, startZ, tmp);
}


// in order to turn around the whole cube, we always turn it by 180 degrees VERTICALLY (along x axis)
void Cube::spinUpAlongX180() {
    unsigned x, y, z;

    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (y == 1 && z == 1) // core piece (invisible) is reached -> any piece has been changed -> done
                    return;
                if (cubePieces[x][y][z].isCornerPiece()) {
                    swapPieces(x, y, z, x, 2-y, 2-z); // 2-y and 2-z swaps start and target coordinates of y,z
                } else if (cubePieces[x][y][z].isEdgePiece()) {
                    if (x == 1) {   // x == 1 means that edge pieces are swapping diagonally
                        swapPieces(x, 2-y, 2-z, x, y, z);
                    } else if ((x == 0 && y == 1) || (x == 2 && y == 1)) {  // edge pieces are on x == 0 \ 2
                        swapPieces(x, y, z, x, y, 2-z);
                    } else {
                        swapPieces(x, y, z, x, 2-y, z);
                    }
                } else {
                    if (x == 1) {
                        if (z == 0)
                            swapPieces(x, y, z, x, y, 2-z);
                        else
                            swapPieces(x, y, z, x, 2-y, z);
                    }
                }
            }
        }
    }
}

// spins whole cube to the right along the z axis
void Cube::spinRight90AlongZ() {
    spinLayerRight90AlongZ(0);
    spinLayerRight90AlongZ(1);
    spinLayerRight90AlongZ(2);
}

// spins the affected layer (0: bottom - 1: middle - 2: top) 90 degrees to the right along z axis
void Cube::spinLayerRight90AlongZ(unsigned layer) {
    unsigned x, y;
    unsigned z = layer;
    CubePiece tmpEdge, tmpCorn;

    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMoveRight90();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMoveRight90();
        }
    }

    // if (layer != 1) {
        // moving corner pieces
        tmpCorn = cubePieces[0][2][z];
        cubePieces[0][2][z] = cubePieces[0][0][z];
        cubePieces[0][0][z] = cubePieces[2][0][z];
        cubePieces[2][0][z] = cubePieces[2][2][z];
        cubePieces[2][2][z] = tmpCorn;

        // moving edge cubePieces
        tmpEdge = cubePieces[0][1][z];
        cubePieces[0][1][z] = cubePieces[1][0][z];
        cubePieces[1][0][z] = cubePieces[2][1][z];
        cubePieces[2][1][z] = cubePieces[1][2][z];
        cubePieces[1][2][z] = tmpEdge;
    // }
    // else {
    //     tmpEdge = cubePieces[0][2][z];
    //     cubePieces[0][2][z] =
    // }

}


/**********************************************************************
**                    GLOBAL FUNCTION DEFINITIONS                    **
**********************************************************************/



// testing function
int test() {
    vector<CubePiece> testCube;

    // first layer:
    CubePiece c000 = CubePiece("bwo");
    CubePiece c100 = CubePiece("yb");
    CubePiece c200 = CubePiece("ybr");
    CubePiece c010 = CubePiece("gw");
    CubePiece c110 = CubePiece("w");
    CubePiece c210 = CubePiece("rb");
    CubePiece c020 = CubePiece("rwg");
    CubePiece c120 = CubePiece("gr");
    CubePiece c220 = CubePiece("brw");

    testCube.push_back(c000);
    testCube.push_back(c100);
    testCube.push_back(c200);
    testCube.push_back(c010);
    testCube.push_back(c110);
    testCube.push_back(c210);
    testCube.push_back(c020);
    testCube.push_back(c120);
    testCube.push_back(c220);

    // second layer:
    CubePiece c001 = CubePiece("ob");
    CubePiece c101 = CubePiece("r");
    CubePiece c201 = CubePiece("wo");
    CubePiece c011 = CubePiece("b");
    CubePiece c111 = CubePiece("-");
    CubePiece c211 = CubePiece("g");
    CubePiece c021 = CubePiece("wb");
    CubePiece c121 = CubePiece("o");
    CubePiece c221 = CubePiece("ry");

    testCube.push_back(c001);
    testCube.push_back(c101);
    testCube.push_back(c201);
    testCube.push_back(c011);
    testCube.push_back(c111);
    testCube.push_back(c211);
    testCube.push_back(c021);
    testCube.push_back(c121);
    testCube.push_back(c221);

    // third layer:
    CubePiece c002 = CubePiece("gyr");
    CubePiece c102 = CubePiece("yg");
    CubePiece c202 = CubePiece("oyb");
    CubePiece c012 = CubePiece("wr");
    CubePiece c112 = CubePiece("y");
    CubePiece c212 = CubePiece("oy");
    CubePiece c022 = CubePiece("wgo");
    CubePiece c122 = CubePiece("og");
    CubePiece c222 = CubePiece("goy");

    testCube.push_back(c002);
    testCube.push_back(c102);
    testCube.push_back(c202);
    testCube.push_back(c012);
    testCube.push_back(c112);
    testCube.push_back(c212);
    testCube.push_back(c022);
    testCube.push_back(c122);
    testCube.push_back(c222);

    Cube cube = Cube(testCube);

    // cube.printWholeCube();
    //
    // cube.spinUpAlongX180();
    // cout << "Turned cube 180 degrees vertically\n";
    //
    // cube.printWholeCube();

    cube.printWholeCube();
    cube.spinRight90AlongZ();
    cout << "Spinned cube 90 degrees to the right" << endl;

    cube.printWholeCube();



    return 0;
}

int main() {
    test();
}
