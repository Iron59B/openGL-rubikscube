/* standard includes */
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include "rubikscube.h"

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
//#include<GL/glew.h>
/* SOIL is used for loading (texture) images */
//#include<SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
//#include<GLFW/glfw3.h>

using namespace std;


/**********************************************************************
**                      CLASS MEMBER DEFINITIONS                     **
**********************************************************************/

Color::Color(){}

Color::Color(char color) {
    this->color = color;
}

char Color::getColorChar() {
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
            cout << out << colors[i].getColorChar() << "\n";
            return;
        }
        out = out + colors[i].getColorChar() + ", ";
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

// prepareCornerPieceMove functions

/************************************************************************************************
** functions that turn the color order of the CubePiece so it is oriented correctly            **
** after the particular move                                                                   **
************************************************************************************************/
void CubePiece::prepareCornerPieceMove90AlongX() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();
    char initCol3 = getColors().at(2).getColorChar();
    prepared = prepared + initCol2 + initCol1 + initCol3;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongX(unsigned z, unsigned x) {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();

    // orientation only changes in the particular case
    if (z == 0 || z == 2) {  // top and bottom layer behave differently than middle one
        if (x == 1) {
            prepared = prepared + initCol2 + initCol1;
            setColors(prepared);
        }
    }
}

void CubePiece::prepareCornerPieceMove90AlongY() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();
    char initCol3 = getColors().at(2).getColorChar();
    prepared = prepared + initCol1 + initCol3 + initCol2;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongY(unsigned y) {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();

    // orientation only changes in the second y layer
    if (y == 1) {
        prepared = prepared + initCol2 + initCol1;
        setColors(prepared);
    }
}

// changes the color orientation of the cubepiece so it ist correctly oriented after the move
void CubePiece::prepareCornerPieceMove90AlongZ() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();
    char initCol3 = getColors().at(2).getColorChar();
    prepared = prepared + initCol3 + initCol2 + initCol1;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongZ() {
    string prepared = "";
    char initCol1 = getColors().at(0).getColorChar();
    char initCol2 = getColors().at(1).getColorChar();
    prepared = prepared + initCol2 + initCol1;
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
// // function writes new cubePiece on cubePieces[x][y][z] and returns old one
// CubePiece Cube::setNewPiece(unsigned x, unsigned y, unsigned z, CubePiece cubePiece) {
//     CubePiece tmp = cubePieces[x][y][z];
//     cubePieces[x][y][z] = cubePiece;
//     return tmp;
// }

// // swaps CubePiece[startX][startY][startZ] with CubePiece[targetX][targetY][targetZ]
// void Cube::swapPieces(unsigned startX, unsigned startY, unsigned startZ, unsigned targetX, unsigned targetY, unsigned targetZ) {
//     CubePiece tmp = setNewPiece(targetX, targetY, targetZ, cubePieces[startX][startY][startZ]);
//     setNewPiece(startX, startY, startZ, tmp);
// }


/**********************************************************************
**                  CUBE MOVEMENT FUNCTION DEFINITIONS               **
**********************************************************************/


void Cube::spinUp180AlongX() {
    spinUp90AlongX();
    spinUp90AlongX();
}

// spins whole cube up along the x axis
void Cube::spinUp90AlongX() {
    spinLayerUp90AlongX(0);
    spinLayerUp90AlongX(1);
    spinLayerUp90AlongX(2);
}

// spins affected layer up 90 degrees along the x axis
void Cube::spinLayerUp90AlongX(unsigned xLayer) {
    unsigned y, z;
    unsigned x = xLayer;
    CubePiece tmp;

    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongX();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongX(z, x);
        }
    }

    // moving corner pieces
    tmp = cubePieces[x][0][2];
    cubePieces[x][0][2] = cubePieces[x][0][0];
    cubePieces[x][0][0] = cubePieces[x][2][0];
    cubePieces[x][2][0] = cubePieces[x][2][2];
    cubePieces[x][2][2] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[x][0][1];
    cubePieces[x][0][1] = cubePieces[x][1][0];
    cubePieces[x][1][0] = cubePieces[x][2][1];
    cubePieces[x][2][1] = cubePieces[x][1][2];
    cubePieces[x][1][2] = tmp;
}

// spins whole cube down along the x axis
void Cube::spinDown90AlongX() {
    spinLayerDown90AlongX(0);
    spinLayerDown90AlongX(1);
    spinLayerDown90AlongX(2);
}

// spins affected layer down 90 degrees along the x axis
void Cube::spinLayerDown90AlongX(unsigned xLayer) {
    unsigned y, z;
    unsigned x = xLayer;
    CubePiece tmp;

    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongX();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongX(z, x);
        }
    }

    // moving corner pieces
    tmp = cubePieces[x][2][0];
    cubePieces[x][2][0] = cubePieces[x][0][0];
    cubePieces[x][0][0] = cubePieces[x][0][2];
    cubePieces[x][0][2] = cubePieces[x][2][2];
    cubePieces[x][2][2] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[x][1][0];
    cubePieces[x][1][0] = cubePieces[x][0][1];
    cubePieces[x][0][1] = cubePieces[x][1][2];
    cubePieces[x][1][2] = cubePieces[x][2][1];
    cubePieces[x][2][1] = tmp;
}

// spins whole cube to the right along the y axis
void Cube::spinRight90AlongY() {
    spinLayerRight90AlongY(0);
    spinLayerRight90AlongY(1);
    spinLayerRight90AlongY(2);
}

// spins the affected layer 90 degrees to the right along y axis
void Cube::spinLayerRight90AlongY(unsigned yLayer) {
    unsigned x, z;
    unsigned y = yLayer;
    CubePiece tmp;

    for (z = 0; z < 3; z++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongY();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongY(y);
        }
    }

    // moving corner pieces
    tmp = cubePieces[0][y][2];
    cubePieces[0][y][2] = cubePieces[0][y][0];
    cubePieces[0][y][0] = cubePieces[2][y][0];
    cubePieces[2][y][0] = cubePieces[2][y][2];
    cubePieces[2][y][2] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[0][y][1];
    cubePieces[0][y][1] = cubePieces[1][y][0];
    cubePieces[1][y][0] = cubePieces[2][y][1];
    cubePieces[2][y][1] = cubePieces[1][y][2];
    cubePieces[1][y][2] = tmp;
}

void Cube::spinLeft90AlongY() {
    spinLayerLeft90AlongY(0);
    spinLayerLeft90AlongY(1);
    spinLayerLeft90AlongY(2);
}

void Cube::spinLayerLeft90AlongY(unsigned yLayer) {
    unsigned x, z;
    unsigned y = yLayer;
    CubePiece tmp;

    for (z = 0; z < 3; z++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongY();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongY(y);
        }
    }

    // moving corner pieces
    tmp = cubePieces[2][y][0];
    cubePieces[2][y][0] = cubePieces[0][y][0];
    cubePieces[0][y][0] = cubePieces[0][y][2];
    cubePieces[0][y][2] = cubePieces[2][y][2];
    cubePieces[2][y][2] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[1][y][0];
    cubePieces[1][y][0] = cubePieces[0][y][1];
    cubePieces[0][y][1] = cubePieces[1][y][2];
    cubePieces[1][y][2] = cubePieces[2][y][1];
    cubePieces[2][y][1] = tmp;
}

// spins whole cube to the right along the z axis
void Cube::spinRight90AlongZ() {
    spinLayerRight90AlongZ(0);
    spinLayerRight90AlongZ(1);
    spinLayerRight90AlongZ(2);
}

// spins the affected layer (0: bottom - 1: middle - 2: top) 90 degrees to the right along z axis
void Cube::spinLayerRight90AlongZ(unsigned zLayer) {
    unsigned x, y;
    unsigned z = zLayer;
    CubePiece tmp;

    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongZ();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongZ();
        }
    }

    // moving corner pieces
    tmp = cubePieces[0][2][z];
    cubePieces[0][2][z] = cubePieces[0][0][z];
    cubePieces[0][0][z] = cubePieces[2][0][z];
    cubePieces[2][0][z] = cubePieces[2][2][z];
    cubePieces[2][2][z] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[0][1][z];
    cubePieces[0][1][z] = cubePieces[1][0][z];
    cubePieces[1][0][z] = cubePieces[2][1][z];
    cubePieces[2][1][z] = cubePieces[1][2][z];
    cubePieces[1][2][z] = tmp;
}

// spins whole cube to the left along the z axis
void Cube::spinLeft90AlongZ() {
    spinLayerLeft90AlongZ(0);
    spinLayerLeft90AlongZ(1);
    spinLayerLeft90AlongZ(2);
}

// spins the affected layer (0: bottom - 1: middle - 2: top) 90 degrees to the left along z axis
void Cube::spinLayerLeft90AlongZ(unsigned zLayer) {
    unsigned x, y;
    unsigned z = zLayer;
    CubePiece tmp;

    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece())
                cubePieces[x][y][z].prepareCornerPieceMove90AlongZ();
            else if (cubePieces[x][y][z].isEdgePiece())
                cubePieces[x][y][z].prepareEdgePieceMove90AlongZ();
        }
    }

    // moving corner pieces
    tmp = cubePieces[2][0][z];
    cubePieces[2][0][z] = cubePieces[0][0][z];
    cubePieces[0][0][z] = cubePieces[0][2][z];
    cubePieces[0][2][z] = cubePieces[2][2][z];
    cubePieces[2][2][z] = tmp;

    // moving edge cubePieces
    tmp = cubePieces[1][0][z];
    cubePieces[1][0][z] = cubePieces[0][1][z];
    cubePieces[0][1][z] = cubePieces[1][2][z];
    cubePieces[1][2][z] = cubePieces[2][1][z];
    cubePieces[2][1][z] = tmp;
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

    cube.printWholeCube();

    // cube.spinLeft90AlongZ();
    // cout << "Spinned cube 90 degrees to the left" << endl;

    // cube.spinUp90AlongX();
    // cout << "spinned cube 90 degrees up along x\n";

    // cube.spinDown90AlongX();
    // cout << "spinned cube 90 degrees down along x axis \n ------------------------------- \n\n";

    // cube.spinRight90AlongY();
    // cout << "spinned cube 90 degrees clockwise along y axis \n\n";

    cube.spinLeft90AlongY();
    cout << "spinned cube 90 degr to the left along y axis\n\n";


    cube.printWholeCube();


    return 0;
}

int main() {
    test();
}
