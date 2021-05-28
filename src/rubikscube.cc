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

char CubePiece::getColor(unsigned index) {
    return colors.at(index).getColorChar();
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
    if (colors.size() == 1 && getColor(0) != '-')
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
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);
    char initCol3 = getColor(2);
    prepared = prepared + initCol2 + initCol1 + initCol3;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongX(unsigned z, unsigned x) {
    string prepared = "";
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);

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
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);
    char initCol3 = getColor(2);
    prepared = prepared + initCol1 + initCol3 + initCol2;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongY(unsigned y) {
    string prepared = "";
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);

    // orientation only changes in the second y layer
    if (y == 1) {
        prepared = prepared + initCol2 + initCol1;
        setColors(prepared);
    }
}

// changes the color orientation of the cubepiece so it ist correctly oriented after the move
void CubePiece::prepareCornerPieceMove90AlongZ() {
    string prepared = "";
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);
    char initCol3 = getColor(2);
    prepared = prepared + initCol3 + initCol2 + initCol1;
    setColors(prepared);
}

void CubePiece::prepareEdgePieceMove90AlongZ() {
    string prepared = "";
    char initCol1 = getColor(0);
    char initCol2 = getColor(1);
    prepared = prepared + initCol2 + initCol1;
    setColors(prepared);
}

// returns true if the corresponding color is on top (or bottom) of the cube (on one of the edge pieces)
bool CubePiece::isColorOnTopOfEdgePiece(char color, unsigned x) {
    if (x == 0 || x == 2) {
        if (getColor(0) == color)
            return true;
    }
    else if (x == 1) {
        if (getColor(1) == color)
            return true;
    }

    return false;
}

bool CubePiece::edgePieceContainsColor(char color) {
    if (getColor(0) == 'w')
        return true;
    else if (getColor(1) == 'w')
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


void Cube::buildWhiteFlowerMiddleLayer() {
    unsigned x, y, z;
    unsigned counter;
    z = 1;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                if (cubePieces[x][y][z].getColor(0) == 'w') { // means that the white face is on front or back side
                    counter = 0;
                    while (cubePieces[x][1][2].getColor(0) == 'w' && counter < 4) { // layer can be turned only if target piece is not white
                        if (counter == 4)
                            return;
                        spinLayerRight90AlongZ(2);
                        counter++;
                    }
                    if (cubePieces[x][1][2].getColor(0) != 'w') {
                        if (y == 0) // front y layer
                            spinLayerUp90AlongX(x);
                        else if (y == 2) // back y layer
                            spinLayerDown90AlongX(x);
                    }
                    else
                        return;
                }
                else if (cubePieces[x][y][z].getColor(1) == 'w') { // means that the white face is lateral
                    counter = 0;
                    while (cubePieces[1][y][2].getColor(1) == 'w' && counter < 4) {
                        if (counter == 4)
                            return;
                        spinLayerRight90AlongZ(2);
                        counter++;
                    }
                    if (cubePieces[1][y][2].getColor(1) != 'w') {
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
}

void Cube::buildWhiteFlowerBottomLayer() {
    unsigned x, y, z;
    unsigned counter;
    if (isWhiteFlowerOnTop())
        return;
    z = 0; // bottom z-layer a bit more complicated
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                if (x == 0 || x == 2) { // lateral x layers
                    if (cubePieces[x][y][z].getColor(0) == 'w') { // means that the white face is on bottom of the cube
                        counter = 0;
                        while (cubePieces[x][1][2].getColor(0) == 'w' && counter < 5) {
                            if (counter == 4)
                                return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[x][1][2].getColor(0) != 'w') {
                            // turn the layer 180 degrees and insert piece in opposite side of cube
                            spinLayerUp90AlongX(x);
                            spinLayerUp90AlongX(x);
                        }
                        else
                            return;
                    }
                    else if (cubePieces[x][y][z].getColor(1) == 'w') { // white face is not on the bottom but on the side --> we need to turn the layer if possible and then insert like in z=1
                        counter = 0;
                        while (cubePieces[x][1][2].getColor(0) == 'w' && counter < 5) {
                            if (counter == 4)
                                return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[x][1][2].getColor(0) != 'w') {
                            spinLayerUp90AlongX(x); // piece gets inserted in top layer in the next loop
                        }
                        else
                            return;
                    }
                }
                else if (x == 1) {
                    if (cubePieces[x][y][z].getColor(1) == 'w') { // means that the white face is on bottom of the cube
                        counter = 0;
                        while (cubePieces[1][y][2].getColor(1) == 'w' && counter < 5) {
                            if (counter == 4)
                                return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[1][y][2].getColor(1) != 'w') {
                            spinLayerRight90AlongY(y);
                            spinLayerRight90AlongY(y);
                        }
                        else {
                            return;
                        }
                    }
                    else if (cubePieces[x][y][z].getColor(0) == 'w') { // white face is not on the bottom but on the side --> we need to turn the layer if possible and then insert like in z=1
                        counter = 0;
                        while (cubePieces[1][y][2].getColor(1) == 'w' && counter < 5) {
                            if (counter == 4)
                                return;
                            spinLayerRight90AlongZ(2);
                            counter++;
                        }
                        if (cubePieces[1][y][2].getColor(0) != 'w') {
                            spinLayerRight90AlongY(y); // piece gets inserted in top layer in the next loop
                        }
                        else return;
                    }
                }
            }
        }
    }
}

void Cube::buildWhiteFlowerTopLayer() {
    unsigned x, y, z;
    if (isWhiteFlowerOnTop())
        return;
    z = 2;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                if (x == 0 || x == 2) {
                    if (cubePieces[x][y][z].getColor(1) == 'w')
                        spinLayerDown90AlongX(x);
                }
                else if (x == 1) {
                    if (cubePieces[x][y][z].getColor(0) == 'w')
                        spinLayerRight90AlongY(y);
                }
            }
        }
    }
}

void Cube::buildWhiteFlower() {
    turnCubeYellowTop();
    unsigned counter = 0;

    while (!isWhiteFlowerOnTop()) {
        buildWhiteFlowerMiddleLayer(); // middle z-layer first (only 1 move to insert in top layer)
        buildWhiteFlowerBottomLayer(); // bring bottom pieces in middle layer in order to insert them like above
        buildWhiteFlowerTopLayer();
        counter++;
    }
}

void Cube::buildWhiteCross() {
    unsigned x, y, z;
    unsigned counter;

    if (!isWhiteFlowerOnTop())
        buildWhiteFlower();

    while (!isWhiteCrossOnBottom()) {
        z = 1;
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isSurfacePiece() && x == 1) {
                    counter = 0;
                    while (cubePieces[x][y][z].getColor(0) != cubePieces[x][y][z+1].getColor(0) || cubePieces[x][y][z+1].isColorOnTopOfEdgePiece('w', x) == false) {
                        if (counter > 4) {
                            if (isWhiteCrossOnBottom())
                                return;
                        }
                        spinLayerRight90AlongZ(z);
                        counter++;
                    }
                    spinLayerRight90AlongY(y);
                    spinLayerRight90AlongY(y);
                }
                else if (cubePieces[x][y][z].isSurfacePiece() && (x == 0 || x == 2)) {
                    while (cubePieces[x][y][z].getColor(0) != cubePieces[x][y][z+1].getColor(1) || cubePieces[x][y][z+1].isColorOnTopOfEdgePiece('w', x) == false) {
                        if (counter > 4) {
                            if (isWhiteCrossOnBottom())
                                return;
                        }
                        spinLayerRight90AlongZ(z);
                        counter++;
                    }
                    spinLayerDown90AlongX(x);
                    spinLayerDown90AlongX(x);
                }
            }
        }
        cout << "is White cross on bottom: " << isWhiteCrossOnBottom() << endl;
    }
}

void Cube::solveFirstLayer() {

}

// returns true if top layer forms the white flower
bool Cube::isWhiteFlowerOnTop() {
    if (cubePieces[1][1][2].getColor(0) == 'y') {
        if (cubePieces[1][0][2].isColorOnTopOfEdgePiece('w', 1) && cubePieces[0][1][2].isColorOnTopOfEdgePiece('w', 0)
        && cubePieces[2][1][2].isColorOnTopOfEdgePiece('w', 2) && cubePieces[1][2][2].isColorOnTopOfEdgePiece('w', 1))
            return true;
    }
    return false;
}

// returns true if bottom layer forms the white cross
bool Cube::isWhiteCrossOnBottom() {
    if (cubePieces[1][1][0].getColor(0) == 'w') {
        if (cubePieces[1][0][0].isColorOnTopOfEdgePiece('w', 1) && cubePieces[0][1][0].isColorOnTopOfEdgePiece('w', 0)
        && cubePieces[2][1][0].isColorOnTopOfEdgePiece('w', 2) && cubePieces[1][2][0].isColorOnTopOfEdgePiece('w', 1))
            return true;
    }
    return false;
}

// turns the cube so the yellow layer is on top
void Cube::turnCubeYellowTop() {

    if (cubePieces[1][1][2].getColor(0) != 'y') {
        if (cubePieces[1][1][0].getColor(0) == 'y')
            spinUp180AlongX();
        else if (cubePieces[0][1][1].getColor(0) == 'y')
            spinRight90AlongY();
        else if (cubePieces[2][1][1].getColor(0) == 'y')
            spinLeft90AlongY();
        else if (cubePieces[1][0][1].getColor(0) == 'y')
            spinUp90AlongX();
        else if (cubePieces[1][2][1].getColor(0) == 'y')
            spinDown90AlongX();
    }
}

void Cube::createRandomCube() {
    srand(time(NULL));
    unsigned randomLoops = rand() % 30 + 1;
    unsigned randomMove;
    unsigned randomNrRotations;
    unsigned randomLayer;
    unsigned i, p;

    cout << randomLoops << endl;

    for (i = 0; i < randomLoops; i++) {
        randomMove = rand() % 6;
        randomNrRotations = rand() % 3 + 1;
        randomLayer = rand() % 3;
        switch(randomMove) {
            case 0:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerUp90AlongX(randomLayer);
                cout << "spinned layer up 90 along x" << endl;
                break;
            case 1:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerDown90AlongX(randomLayer);
                cout << "spinned layer down 90 along x" << endl;
                break;
            case 2:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerRight90AlongY(randomLayer);
                cout << "spinned layer right 90 along y" << endl;
                break;
            case 3:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerLeft90AlongY(randomLayer);
                cout << "spinned layer left 90 along y" << endl;
                break;
            case 4:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerRight90AlongZ(randomLayer);
                cout << "spinned layer right 90 along z" << endl;
                break;
            case 5:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerLeft90AlongZ(randomLayer);
                cout << "spinned layer left 90 along z" << endl;
                break;
        }
    }

}


/**********************************************************************
**                    GLOBAL FUNCTION DEFINITIONS                    **
**********************************************************************/



// testing function
int test() {
    vector<CubePiece> testCube;

    // first layer:
    CubePiece c000 = CubePiece("rwb");
    CubePiece c100 = CubePiece("rw");
    CubePiece c200 = CubePiece("rwg");
    CubePiece c010 = CubePiece("wb");
    CubePiece c110 = CubePiece("w");
    CubePiece c210 = CubePiece("wg");
    CubePiece c020 = CubePiece("owb");
    CubePiece c120 = CubePiece("ow");
    CubePiece c220 = CubePiece("owg");

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
    CubePiece c001 = CubePiece("rb");
    CubePiece c101 = CubePiece("r");
    CubePiece c201 = CubePiece("rg");
    CubePiece c011 = CubePiece("b");
    CubePiece c111 = CubePiece("-");
    CubePiece c211 = CubePiece("g");
    CubePiece c021 = CubePiece("ob");
    CubePiece c121 = CubePiece("o");
    CubePiece c221 = CubePiece("og");

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
    CubePiece c002 = CubePiece("ryb");
    CubePiece c102 = CubePiece("ry");
    CubePiece c202 = CubePiece("ryg");
    CubePiece c012 = CubePiece("yb");
    CubePiece c112 = CubePiece("y");
    CubePiece c212 = CubePiece("yg");
    CubePiece c022 = CubePiece("oyb");
    CubePiece c122 = CubePiece("ob");
    CubePiece c222 = CubePiece("oyg");

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

    cube.createRandomCube();
    //cube.printWholeCube();

    cube.buildWhiteCross();

    cube.printFirstLayer();

    return 0;
}

int main() {
    test();
}
