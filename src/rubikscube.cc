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

// returns true if the corresponding color is on top of the cube (on one of the edge pieces)
bool CubePiece::isColorOnTopOfEdgePiece(char color, unsigned x, unsigned y) {
    if (x == 0 || x == 2) {
        if (getColors().at(0).getColorChar() == color)
            return true;
    }
    else if (x == 1) {
        if (getColors().at(1).getColorChar() == color)
            return true;
    }

    return false;
}

bool CubePiece::edgePieceContainsColor(char color) {
    if (getColors().at(0).getColorChar() == 'w')
        return true;
    else if (getColors().at(1).getColorChar() == 'w')
        return true;
    else
        return false;
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


/**********************************************************************
**                  CUBE MOVEMENT FUNCTION DEFINITIONS               **
**********************************************************************/

// any identifier corresponds to the point of view of the beholder which means that [0][0][0] is the front-bottom-leftest cube piece

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

// spins affected layer (0: left, 1: middle, 2: right) up 90 degrees along the x axis
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

// spins affected layer (0: left, 1: middle, 2: right) down 90 degrees along the x axis
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
**                    CUBE SOLVING ALGORITHM SECTION                 **
**********************************************************************/

void Cube::solveFirstLayer() {

}

void Cube::buildWhiteCross() {
 
}

void Cube::buildWhiteFlower() {
    unsigned x, y, z;
    unsigned counter;

    turnCubeYellowTop();

    while (isWhiteFlowerOnTop() == false) {
        z = 1; // middle z-layer first (only 1 move to insert in top layer)
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                    if (cubePieces[x][y][z].getColors().at(0).getColorChar() == 'w') { // means that the white face is on front or back side
                        counter = 0;
                        cout << "was in there" << endl;
                        while (cubePieces[x][1][2].getColors().at(0).getColorChar() == 'w' && counter < 4) { // layer can be turned only if target piece is not white
                            if (counter == 4)
                                    return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[x][1][2].getColors().at(0).getColorChar() != 'w') {
                            if (y == 0) // front y layer
                                spinLayerUp90AlongX(x);
                            else if (y == 2) // back y layer
                                spinLayerDown90AlongX(x);
                        }
                        else
                            return;
                    }
                    else if (cubePieces[x][y][z].getColors().at(1).getColorChar() == 'w') { // means that the white face is lateral
                        counter = 0;
                        while (cubePieces[1][y][2].getColors().at(1).getColorChar() == 'w' && counter < 4) {
                            if (counter == 4)
                                    return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[1][y][2].getColors().at(1).getColorChar() != 'w') {
                            if (x == 0) // left x layer
                                spinLayerRight90AlongY(y);
                            else if (x == 2) // right x layer
                                spinLayerLeft90AlongY(y);
                        }
                        else
                            return;
                    }
                }
            }
        }

        if (isWhiteFlowerOnTop())
            return;
        z = 0; // bottom z-layer a bit more complicated
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                    if (x == 0 || x == 2) { // lateral x layers
                        if (cubePieces[x][y][z].getColors().at(0).getColorChar() == 'w') { // means that the white face is on bottom of the cube
                            counter = 0;
                            while (cubePieces[x][1][2].getColors().at(0).getColorChar() == 'w' && counter < 5) {
                                if (counter == 4)
                                    return;
                                spinLayerRight90AlongZ(2);
                                counter++;
                            }
                            if (cubePieces[x][1][2].getColors().at(0).getColorChar() != 'w') {
                                // turn the layer 180 degrees and insert piece in opposite side of cube
                                spinLayerUp90AlongX(x);
                                spinLayerUp90AlongX(x);
                            }
                            else
                                return;
                        }
                        else if (cubePieces[x][y][z].getColors().at(1).getColorChar() == 'w') { // white face is not on the bottom but on the side --> we need to turn the layer if possible and then insert like in z=1
                            counter = 0;
                            while (cubePieces[x][1][2].getColors().at(0).getColorChar() == 'w' && counter < 5) {
                                if (counter == 4)
                                    return;
                                spinLayerRight90AlongZ(2);
                                counter++;
                            }
                            if (cubePieces[x][1][2].getColors().at(0).getColorChar() != 'w') {
                                spinLayerUp90AlongX(x); // piece gets inserted in top layer in the next loop
                            }
                            else
                                return;
                        }
                    }
                    else if (x == 1) {
                        if (cubePieces[x][y][z].getColors().at(1).getColorChar() == 'w') { // means that the white face is on bottom of the cube
                            counter = 0;
                            while (cubePieces[1][y][2].getColors().at(1).getColorChar() == 'w' && counter < 5) {
                                if (counter == 4)
                                    return;
                                spinLayerRight90AlongZ(2);
                                counter++;
                            }
                            if (cubePieces[1][y][2].getColors().at(1).getColorChar() != 'w') {
                                spinLayerRight90AlongY(y);
                                spinLayerRight90AlongY(y);
                            }
                            else
                                return;
                        }
                        else if (cubePieces[x][y][z].getColors().at(0).getColorChar() == 'w') { // white face is not on the bottom but on the side --> we need to turn the layer if possible and then insert like in z=1
                            counter = 0;
                            while (cubePieces[1][y][2].getColors().at(1).getColorChar() == 'w' && counter < 5) {
                                if (counter == 4)
                                    return;
                                spinLayerRight90AlongZ(2);
                                counter++;
                            }
                            if (cubePieces[1][y][2].getColors().at(0).getColorChar() != 'w') {
                                spinLayerUp90AlongX(x); // piece gets inserted in top layer in the next loop
                            }
                            else return;
                        }
                    }
                }
            }
        }

        if (isWhiteFlowerOnTop())
            return;
        z = 2;
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                    if (x == 0 || x == 2) {
                        if (cubePieces[x][y][z].getColors().at(1).getColorChar() == 'w')
                            spinLayerDown90AlongX(x);
                    }
                    else if (x == 1) {
                        if (cubePieces[x][y][z].getColors().at(0).getColorChar() == 'w')
                            spinLayerRight90AlongY(y);
                    }
                }
            }
        }
    }
}

bool Cube::isWhiteFlowerOnTop() {
    if (cubePieces[1][1][2].getColors().at(0).getColorChar() == 'y')
        if (cubePieces[1][0][2].isColorOnTopOfEdgePiece('w', 1, 0) && cubePieces[0][1][2].isColorOnTopOfEdgePiece('w', 0, 1)
        && cubePieces[2][1][2].isColorOnTopOfEdgePiece('w', 2, 1) && cubePieces[1][2][2].isColorOnTopOfEdgePiece('w', 1, 2))
            return true;
    
    return false;
}

// turns the cube so the yellow layer is on top
void Cube::turnCubeYellowTop() {

    if (cubePieces[1][1][2].getColors().at(0).getColorChar() != 'y') {
        if (cubePieces[1][1][0].getColors().at(0).getColorChar() == 'y')
            spinUp180AlongX();
        else if (cubePieces[0][1][1].getColors().at(0).getColorChar() == 'y')
            spinRight90AlongY();
        else if (cubePieces[2][1][1].getColors().at(0).getColorChar() == 'y')
            spinLeft90AlongY();
        else if (cubePieces[1][0][1].getColors().at(0).getColorChar() == 'y')
            spinUp90AlongX();
        else if (cubePieces[1][2][1].getColors().at(0).getColorChar() == 'y')
            spinDown90AlongX();
    }
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

    cube.buildWhiteFlower();

    cube.printWholeCube();

    return 0;
}

int main() {
    test();
}
