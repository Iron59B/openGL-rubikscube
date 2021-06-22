/* standard includes */
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include <array>
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

string CubePiece::getColors() {
    unsigned i;
    string ret = "";

    for (i = 0; i < colors.size(); i++) {
        ret = ret + colors.at(i).getColorChar();
    }

    return ret;
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

// returns true if the piece holds the corresponding color
bool CubePiece::edgePieceContainsColor(char color) {
    if (getColor(0) == color)
        return true;
    else if (getColor(1) == color)
        return true;
    else
        return false;
}

// returns the position of the color in the cubepiece, -1 if color is not held by piece
int CubePiece::getPositionOfColor(char color) {
    if (isSurfacePiece()) {
        if (getColor(0) == color)
            return 0;
        else
            return -1;
    }
    else if (isEdgePiece()) {
        if (getColor(0) == color)
            return 0;
        else if (getColor(1) == color)
            return 1;
        else
            return -1;
    }
    else if (isCornerPiece()) {
        if (getColor(0) == color)
            return 0;
        else if (getColor(1) == color)
            return 1;
        else if (getColor(2) == color)
            return 2;
        else
            return -1;
    }
    return -1;
}


///////////////////////////////////////////////////////////////////////////////


AlgoCube::AlgoCube() {}

AlgoCube::AlgoCube(vector<CubePiece> pieces) {
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

vector<int> AlgoCube::getMoves() {
    return moves;
}

vector<int> AlgoCube::getRandomizeCubeMoves() {
    return randomizeCubeMoves;
}

void AlgoCube::setPieces(vector<CubePiece> pieces) {
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
void AlgoCube::printFirstLayer() {
    unsigned i, p;
    string out = "";

    for (i = 0; i < 3; i++) {
        for (p = 0; p < 3; p++) {
            cubePieces[p][i][0].print();
            cout << "\n";
        }
    }
}

void AlgoCube::printWholeCube() {
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


// spins whole cube up along the x axis
void AlgoCube::spinUp90AlongX() {
    spinLayerUp90AlongX(0, true);
    spinLayerUp90AlongX(1, true);
    spinLayerUp90AlongX(2, true);

    moves.push_back(0);
}

// spins whole cube down along the x axis
void AlgoCube::spinDown90AlongX() {
    spinLayerDown90AlongX(0, true);
    spinLayerDown90AlongX(1, true);
    spinLayerDown90AlongX(2, true);

    moves.push_back(1);
}

// spins whole cube to the right along the y axis
void AlgoCube::spinRight90AlongY() {
    spinLayerRight90AlongY(0, true);
    spinLayerRight90AlongY(1, true);
    spinLayerRight90AlongY(2, true);

    moves.push_back(2);
}

// spins whole cube to the left along the y axis
void AlgoCube::spinLeft90AlongY() {
    spinLayerLeft90AlongY(0, true);
    spinLayerLeft90AlongY(1, true);
    spinLayerLeft90AlongY(2, true);

    moves.push_back(3);
}

// spins whole cube to the right along the z axis
void AlgoCube::spinRight90AlongZ() {
    spinLayerRight90AlongZ(0, true);
    spinLayerRight90AlongZ(1, true);
    spinLayerRight90AlongZ(2, true);

    moves.push_back(4);
}

// spins whole cube to the left along the z axis
void AlgoCube::spinLeft90AlongZ() {
    spinLayerLeft90AlongZ(0, true);
    spinLayerLeft90AlongZ(1, true);
    spinLayerLeft90AlongZ(2, true);

    moves.push_back(5);
}

// spins affected layer (0: left, 1: middle, 2: right) up 90 degrees along the x axis
void AlgoCube::spinLayerUp90AlongX(unsigned xLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (xLayer) {
            case 0:
                moves.push_back(6);
                break;
            case 1:
                moves.push_back(7);
                break;
            case 2:
                moves.push_back(8);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (xLayer) {
            case 0:
                randomizeCubeMoves.push_back(6);
                break;
            case 1:
                randomizeCubeMoves.push_back(7);
                break;
            case 2:
                randomizeCubeMoves.push_back(8);
                break;
        }
    }
}

// spins affected layer (0: left, 1: middle, 2: right) down 90 degrees along the x axis
void AlgoCube::spinLayerDown90AlongX(unsigned xLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (xLayer) {
            case 0:
                moves.push_back(9);
                break;
            case 1:
                moves.push_back(10);
                break;
            case 2:
                moves.push_back(11);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (xLayer) {
            case 0:
                randomizeCubeMoves.push_back(9);
                break;
            case 1:
                randomizeCubeMoves.push_back(10);
                break;
            case 2:
                randomizeCubeMoves.push_back(11);
                break;
        }
    }
}

// spins the affected layer 90 degrees to the right along y axis
void AlgoCube::spinLayerRight90AlongY(unsigned yLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (yLayer) {
            case 0:
                moves.push_back(12);
                break;
            case 1:
                moves.push_back(13);
                break;
            case 2:
                moves.push_back(14);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (yLayer) {
            case 0:
                randomizeCubeMoves.push_back(12);
                break;
            case 1:
                randomizeCubeMoves.push_back(13);
                break;
            case 2:
                randomizeCubeMoves.push_back(14);
                break;
        }
    }
}

void AlgoCube::spinLayerLeft90AlongY(unsigned yLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (yLayer) {
            case 0:
                moves.push_back(15);
                break;
            case 1:
                moves.push_back(16);
                break;
            case 2:
                moves.push_back(17);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (yLayer) {
            case 0:
                randomizeCubeMoves.push_back(15);
                break;
            case 1:
                randomizeCubeMoves.push_back(16);
                break;
            case 2:
                randomizeCubeMoves.push_back(17);
                break;
        }
    }
}

// spins the affected layer (0: bottom - 1: middle - 2: top) 90 degrees to the right along z axis
void AlgoCube::spinLayerRight90AlongZ(unsigned zLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (zLayer) {
            case 0:
                moves.push_back(18);
                break;
            case 1:
                moves.push_back(19);
                break;
            case 2:
                moves.push_back(20);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (zLayer) {
            case 0:
                randomizeCubeMoves.push_back(18);
                break;
            case 1:
                randomizeCubeMoves.push_back(19);
                break;
            case 2:
                randomizeCubeMoves.push_back(20);
                break;
        }
    }
}

// spins the affected layer (0: bottom - 1: middle - 2: top) 90 degrees to the left along z axis
void AlgoCube::spinLayerLeft90AlongZ(unsigned zLayer, bool wholeCube, int forRandomize) {
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

    if (wholeCube == false && forRandomize == 0) {
        switch (zLayer) {
            case 0:
                moves.push_back(21);
                break;
            case 1:
                moves.push_back(22);
                break;
            case 2:
                moves.push_back(23);
                break;
        }
    }
    if (forRandomize == 1) {
        switch (zLayer) {
            case 0:
                randomizeCubeMoves.push_back(21);
                break;
            case 1:
                randomizeCubeMoves.push_back(22);
                break;
            case 2:
                randomizeCubeMoves.push_back(23);
                break;
        }
    }
}


/**********************************************************************
**                    CUBE SOLVING ALGORITHM SECTION                 **
**********************************************************************/


/***    	       SOLVE FIRST LAYER            ***/


void AlgoCube::buildWhiteFlowerMiddleLayer() {
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
                        spinLayerRight90AlongZ(2, false);
                        counter++;
                    }
                    if (cubePieces[x][1][2].getColor(0) != 'w') {
                        if (y == 0) { // front y layer
                            spinLayerUp90AlongX(x, false);
                        }
                        else if (y == 2) { // back y layer
                            spinLayerDown90AlongX(x, false);
                        }
                    }
                    else
                        return;
                }
                else if (cubePieces[x][y][z].getColor(1) == 'w') { // means that the white face is lateral
                    counter = 0;
                    while (cubePieces[1][y][2].getColor(1) == 'w' && counter < 4) {
                        if (counter == 4)
                            return;
                        spinLayerRight90AlongZ(2, false);
                        counter++;
                    }
                    if (cubePieces[1][y][2].getColor(1) != 'w') {
                        if (x == 0) { // left x layer
                            spinLayerRight90AlongY(y, false);
                        }
                        else if (x == 2) { // right x layer
                            spinLayerLeft90AlongY(y, false);
                        }
                    }
                    else
                        return;
                }
            }
        }
    }
}

void AlgoCube::buildWhiteFlowerBottomLayer() {
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
                            spinLayerRight90AlongZ(2, false);
                            counter++;
                        }
                        if (cubePieces[x][1][2].getColor(0) != 'w') {
                            // turn the layer 180 degrees and insert piece in opposite side of cube
                            spinLayerUp90AlongX(x, false);
                            spinLayerUp90AlongX(x, false);
                        }
                        else
                            return;
                    }
                    else if (cubePieces[x][y][z].getColor(1) == 'w') { // white face is not on the bottom but on the side --> we need to turn the layer if possible and then insert like in z=1
                        counter = 0;
                        while (cubePieces[x][1][2].getColor(0) == 'w' && counter < 5) {
                            if (counter == 4)
                                return;
                            spinLayerRight90AlongZ(2, false);
                            counter++;
                        }
                        if (cubePieces[x][1][2].getColor(0) != 'w') {
                            spinLayerUp90AlongX(x, false); // piece gets inserted in top layer in the next loop

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
                            spinLayerRight90AlongZ(2, false);
                            counter++;
                        }
                        if (cubePieces[1][y][2].getColor(1) != 'w') {
                            spinLayerRight90AlongY(y, false);
                            spinLayerRight90AlongY(y, false);
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
                            spinLayerRight90AlongZ(2, false);
                            counter++;
                        }
                        if (cubePieces[1][y][2].getColor(0) != 'w') {
                            spinLayerRight90AlongY(y, false); // piece gets inserted in top layer in the next loop

                        }
                        else return;
                    }
                }
            }
        }
    }
}

void AlgoCube::buildWhiteFlowerTopLayer() {
    unsigned x, y, z;
    if (isWhiteFlowerOnTop())
        return;
    z = 2;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('w')) {
                if (x == 0 || x == 2) {
                    if (cubePieces[x][y][z].getColor(1) == 'w') {
                        spinLayerDown90AlongX(x, false);
                    }
                }
                else if (x == 1) {
                    if (cubePieces[x][y][z].getColor(0) == 'w') {
                        spinLayerRight90AlongY(y, false);
                    }
                }
            }
        }
    }
}

void AlgoCube::buildWhiteFlower() {
    turnCubeYellowTop();
    unsigned counter = 0;

    while (!isWhiteFlowerOnTop()) {
        buildWhiteFlowerMiddleLayer(); // middle z-layer first (only 1 move to insert in top layer)
        buildWhiteFlowerBottomLayer(); // bring bottom pieces in middle layer in order to insert them like above
        buildWhiteFlowerTopLayer();
        counter++;
    }
    cout << "white flower built" << endl;
}

void AlgoCube::buildWhiteCross() {
    unsigned x, y, z;
    unsigned counter;

    if (!isWhiteFlowerOnTop())
        buildWhiteFlower();

    while (!isWhiteCrossOnBottom()) {
        z = 2; // top layer
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isEdgePiece() && x == 1 && cubePieces[x][y][z].isColorOnTopOfEdgePiece('w', x)) {
                    counter = 0;
                    while (cubePieces[x][y][z].getColor(0) != cubePieces[x][y][z-1].getColor(0)) {
                        if (counter > 4) {
                            if (isWhiteCrossOnBottom())
                                return;
                        }
                        spinLayerRight90AlongZ(0, false);
                        spinLayerRight90AlongZ(1, false);
                        counter++;
                    }
                    if (cubePieces[x][y][z].getColor(0) == cubePieces[x][y][z-1].getColor(0)) {
                        spinLayerRight90AlongY(y, false);
                        spinLayerRight90AlongY(y, false);
                    }
                    else {
                        cout << "An error occured." << endl;
                        exit(1);
                    }
                }
                else if (cubePieces[x][y][z].isEdgePiece() && (x == 0 || x == 2) && cubePieces[x][y][z].isColorOnTopOfEdgePiece('w', x)) {
                    counter = 0;
                    while (cubePieces[x][y][z].getColor(1) != cubePieces[x][y][z-1].getColor(0)) {
                        if (counter > 4) {
                            if (isWhiteCrossOnBottom())
                                return;
                        }
                        spinLayerRight90AlongZ(0, false);
                        spinLayerRight90AlongZ(1, false);
                        counter++;
                    }
                    if (cubePieces[x][y][z].getColor(1) == cubePieces[x][y][z-1].getColor(0)) {
                        spinLayerDown90AlongX(x, false);
                        spinLayerDown90AlongX(x, false);
                    }
                    else {
                        cout << "An error occured." << endl;
                        exit(1);
                    }
                }
            }
        }
    }
    cout << "white cross built" << endl;
}

void AlgoCube::solveFirstLayer() {
    unsigned nextCornerX;

    if (!isWhiteCrossOnBottom())
        buildWhiteCross();

    turnCubeWhiteTop();

    while (!isFirstLayerSolved()) {
        nextCornerX = turnCubeUntilWhiteBottomCornerFront();
        insertNextWhiteCornerPiece(nextCornerX);
    }
    cout << "solved first (white) layer" << endl;

}

void AlgoCube::insertNextWhiteCornerPiece(unsigned x) {
    if (x == 0) {               // bottom side                      // front surface                    // lateral side                 // lateral surface
        while (!(cubePieces[x][0][0].getColor(1) == cubePieces[1][0][1].getColor(0) && cubePieces[x][0][0].getColor(2) == cubePieces[x][1][1].getColor(0))) {
            spinLayerRight90AlongZ(2, false);
            spinLayerRight90AlongZ(1, false);
        }
        spinLayerLeft90AlongZ(0, false);
        spinLayerDown90AlongX(x, false);
        spinLayerRight90AlongZ(0, false);
        spinLayerUp90AlongX(x, false);
    }
    else if (x == 2) {
        while (!(cubePieces[x][0][0].getColor(1) == cubePieces[1][0][1].getColor(0) && cubePieces[x][0][0].getColor(2) == cubePieces[x][1][1].getColor(0))) {
            spinLayerRight90AlongZ(2, false);
            spinLayerRight90AlongZ(1, false);
        }
        spinLayerRight90AlongZ(0, false);
        spinLayerDown90AlongX(x, false);
        spinLayerLeft90AlongZ(0, false);
        spinLayerUp90AlongX(x, false);
    }
}

// turns cube until white face of corner piece is in front, returns corresponding x coordinate (0 or 2) of the piece
int AlgoCube::turnCubeUntilWhiteBottomCornerFront() {
    unsigned counter;

    counter = 0;
    while (isWhiteBottomCornerFront() == -1 && counter < 4) { // when there is an outer white face, turn cube until it faces the user
        spinRight90AlongZ();
        counter++;
    }
    if (isWhiteBottomCornerFront() != -1)
        return isWhiteBottomCornerFront();
    else {                    // when there is no such piece, check if there is one on the bottom, else look in top layer for incorrect white corners
        bringBottomWhiteCornerFront();
        if (isWhiteBottomCornerFront() == -1) {
            bringTopWhiteCornerFront();
        }
        else {
            return isWhiteBottomCornerFront();
        }
        if (isWhiteBottomCornerFront() == -1) {
            return -1;
        }
        else {
            return isWhiteBottomCornerFront();
        }
    }
}

// turns bottom layer until a front corner piece has white on bottom, then turns upper 2 layers until target corner is free and finally let white be on front side (through some rotations)
void AlgoCube::bringBottomWhiteCornerFront() {
    unsigned counter;

    counter = 0;
    while (!isWhiteBottomCornerLeftBottom() && counter < 4) {
        spinLayerRight90AlongZ(0, false);
        counter++;
    }
    if (isWhiteBottomCornerLeftBottom()) {
        counter = 0;
        while (isCornerPieceCorrect(0, 0, 'w') && counter < 4) {
            spinLayerRight90AlongZ(2, false);
            spinLayerRight90AlongZ(1, false);
            counter++;
        }
        if (!isCornerPieceCorrect(0, 0, 'w')) {
            spinLayerDown90AlongX(0, false);
            spinLayerRight90AlongZ(0, false);
            spinLayerUp90AlongX(0, false);
            spinLayerRight90AlongZ(0, false); // white face is now on right bottom front
        }
        else
            return; // no bottom faced white corner piece
    }
}

// checks for incorrect white corner pieces in the top layer, if they exist, bring them to bottom layer
void AlgoCube::bringTopWhiteCornerFront() {
    unsigned x, y, z;

    z = 2;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece()) {
                if (!isCornerPieceCorrect(x, y, 'w')) { // when cornerpice is incorrect
                    if (cubePieces[x][y][z].getPositionOfColor('w') != -1) { // has white in it
                        dismantleIncorrectWhiteCornerPiece(x, y); // brings piece to bottom layer where it can be inserted more easily
                    }
                }
            }
        }
    }
}

// puts incorrect corner piece containing white from its current position to the bottom layer (facing out)
// when corner piece appears to be on the back side of the cube (y == 2), cube is being turned twice and x and y change, so it can be inserted more easily
void AlgoCube::dismantleIncorrectWhiteCornerPiece(unsigned x, unsigned y) {
    unsigned z;

    if (y == 2) {
        spinRight90AlongZ();
        spinRight90AlongZ();
        if (x == 0) {
            x = 2;
        }
        else if (x == 2) {
            x = 0;
        }
        y = 0;
    }

    // y == 0
    z = 2;
    if (x == 0) {                           // on top                                        // on the side -> same movements
        if (cubePieces[x][y][z].getPositionOfColor('w') == 1 || cubePieces[x][y][z].getPositionOfColor('w') == 2) {
            spinLayerDown90AlongX(x, false);
            spinLayerLeft90AlongZ(0, false);
            spinLayerUp90AlongX(x, false);
        }
        else if (cubePieces[x][y][z].getPositionOfColor('w') == 0) {
            spinLayerLeft90AlongY(y, false);
            spinLayerRight90AlongZ(0, false);
            spinLayerRight90AlongY(y, false);
        }
        else if (cubePieces[x][y][z].getPositionOfColor('w') == -1) {
            cout << "no white piece to move" << endl;
        }
    }
    else if (x == 2) {
        if (cubePieces[x][y][z].getPositionOfColor('w') == 1 || cubePieces[x][y][z].getPositionOfColor('w') == 2) {
            spinLayerDown90AlongX(x, false);
            spinLayerRight90AlongZ(0, false);
            spinLayerUp90AlongX(x, false);
        }
        else if (cubePieces[x][y][z].getPositionOfColor('w') == 0) {
            spinLayerRight90AlongY(y, false);
            spinLayerLeft90AlongZ(0, false);
            spinLayerLeft90AlongY(y, false);
        }
        else if (cubePieces[x][y][z].getPositionOfColor('w') == -1) {
            cout << "no white piece to move" << endl;
        }
    }
}

// returns 0 if bottom left front corner has a white face and it's on front, 2 if right front corner -"- , else -1
int AlgoCube::isWhiteBottomCornerFront() {
    if (cubePieces[0][0][0].getColor(0) == 'w')
        return 0;
    else if (cubePieces[2][0][0].getColor(0) == 'w')
        return 2;
    else
        return -1;
}

// returns true if the first (white) layer -- which is usually currently on top -- is fully solved, else false
bool AlgoCube::isFirstLayerSolved() {
    unsigned x, y, z;

    turnCubeWhiteTop();

    if (isColorCrossOnTop('w')) {
        z = 2;
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (cubePieces[x][y][z].isCornerPiece()) {
                    if (!isCornerPieceCorrect(x, y, 'w')) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    return false;
}

// turns the cube so the yellow surface piece is on top
void AlgoCube::turnCubeYellowTop() {

    if (cubePieces[1][1][2].getColor(0) != 'y') {
        if (cubePieces[1][1][0].getColor(0) == 'y') {
            spinUp90AlongX();
            spinUp90AlongX();
        }
        else if (cubePieces[0][1][1].getColor(0) == 'y') {
            spinRight90AlongY();
        }
        else if (cubePieces[2][1][1].getColor(0) == 'y') {
            spinLeft90AlongY();
        }
        else if (cubePieces[1][0][1].getColor(0) == 'y') {
            spinUp90AlongX();
        }
        else if (cubePieces[1][2][1].getColor(0) == 'y') {
            spinDown90AlongX();
        }
    }
}

// since function is only being called when white is on bottom, cube is being turned twice along x axis to turn it around
void AlgoCube::turnCubeWhiteTop() {
    if (cubePieces[1][1][2].getColor(0) == 'w')
        return;
    if (cubePieces[1][1][0].getColor(0) == 'w') {
        spinDown90AlongX();
        spinDown90AlongX();
    }
    else {
        turnCubeYellowTop();
        turnCubeWhiteTop();
    }
}

// returns true if bottom left front corner has a white face and it's on the bottom, else false
bool AlgoCube::isWhiteBottomCornerLeftBottom() {
    if (cubePieces[0][0][0].getColor(1) == 'w')
        return true;
    else
        return false;
}

// returns true if corner piece is correct -- top layer (z = 2), else false
bool AlgoCube::isCornerPieceCorrect(unsigned x, unsigned y, char color) {
    if (!cubePieces[x][y][2].isCornerPiece()) {
        cout << "passed piece is no corner piece" << endl;
        exit(1);
    }
    if (x == 0) {
        if (cubePieces[x][y][2].getColor(1) == color && cubePieces[x][y][2].getColor(0) == cubePieces[1][y][1].getColor(0) && cubePieces[x][y][2].getColor(2) == cubePieces[0][1][1].getColor(0))
            return true;
    }
    else if (x == 2) {
        if (cubePieces[x][y][2].getColor(1) == color && cubePieces[x][y][2].getColor(0) == cubePieces[1][y][1].getColor(0) && cubePieces[x][y][2].getColor(2) == cubePieces[2][1][1].getColor(0))
            return true;
    }
    return false;
}

// returns true if top layer forms the white flower
bool AlgoCube::isWhiteFlowerOnTop() {
    if (cubePieces[1][1][2].getColor(0) == 'y') {
        if (cubePieces[1][0][2].isColorOnTopOfEdgePiece('w', 1) && cubePieces[0][1][2].isColorOnTopOfEdgePiece('w', 0)
        && cubePieces[2][1][2].isColorOnTopOfEdgePiece('w', 2) && cubePieces[1][2][2].isColorOnTopOfEdgePiece('w', 1))
            return true;
    }
    return false;
}

// returns true if bottom layer forms the white cross
bool AlgoCube::isWhiteCrossOnBottom() {
    if (cubePieces[1][1][0].getColor(0) == 'w') {
        if (cubePieces[1][0][0].isColorOnTopOfEdgePiece('w', 1) && edgePieceSecondaryMatchesSurface(1, 0, 0) &&
        cubePieces[0][1][0].isColorOnTopOfEdgePiece('w', 0) && edgePieceSecondaryMatchesSurface(0, 1, 0) &&
        cubePieces[2][1][0].isColorOnTopOfEdgePiece('w', 2) && edgePieceSecondaryMatchesSurface(2, 1, 0) &&
        cubePieces[1][2][0].isColorOnTopOfEdgePiece('w', 1) && edgePieceSecondaryMatchesSurface(1, 2, 0))
            return true;
    }
    return false;
}

// returns true if top layer forms the white cross
bool AlgoCube::isColorCrossOnTop(char color) {
    if (cubePieces[1][1][2].getColor(0) == color) {
        if (cubePieces[1][0][2].isColorOnTopOfEdgePiece(color, 1) && edgePieceSecondaryMatchesSurface(1, 0, 2) &&
        cubePieces[0][1][2].isColorOnTopOfEdgePiece(color, 0) && edgePieceSecondaryMatchesSurface(0, 1, 2) &&
        cubePieces[2][1][2].isColorOnTopOfEdgePiece(color, 2) && edgePieceSecondaryMatchesSurface(2, 1, 2) &&
        cubePieces[1][2][2].isColorOnTopOfEdgePiece(color, 1) && edgePieceSecondaryMatchesSurface(1, 2, 2))
            return true;
    }
    return false;
}

// returns true if the secondary color (mostly white primary) matches the color of the surface piece above or below
bool AlgoCube::edgePieceSecondaryMatchesSurface(unsigned x, unsigned y, unsigned z) {
    if (x == 0 || x == 2) {
        if (cubePieces[x][y][z].getColor(1) == cubePieces[x][y][1].getColor(0))
            return true;
    }
    else if (x == 1) {
        if (cubePieces[x][y][z].getColor(0) == cubePieces[x][y][1].getColor(0))
            return true;
    }
    return false;
}


/***    	       SOLVE SECOND LAYER            ***/

void AlgoCube::solveSecondLayer() {
    int nextMoveAwayFrom;

    if (!isFirstLayerSolved())
        solveFirstLayer();

    while (!isSecondLayerSolved()) {
        turnCubeWhiteTop();
        nextMoveAwayFrom = prepareForNextEdgePieceInsertion();
        insertNextEdgePiece(nextMoveAwayFrom);
    }
    cout << "solved second layer" << endl;
}

/* looks for next edge piece to be inserted, returns 0 when next move is away from x=0 layer, or 2 when the next move is away from x=2 layer,
*  if there is no edge piece w/o yellow in it, brings it to bottom layer (yellow)  -- finally returns -1                      */
int AlgoCube::prepareForNextEdgePieceInsertion() {
    unsigned x, y, z;
    unsigned counter;
    char frontColor;
    char bottomColor;

    z = 0;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].edgePieceContainsColor('y') == false) {
                if (x == 1) {
                    frontColor = cubePieces[x][y][z].getColor(0);
                    bottomColor = cubePieces[x][y][z].getColor(1);
                }
                else { // if (x == 0 || x == 2) {
                    frontColor = cubePieces[x][y][z].getColor(1);
                    bottomColor = cubePieces[x][y][z].getColor(0);
                }

                turnCubeColorFront(frontColor);

                while (!(cubePieces[1][0][0].getColor(0) == frontColor && cubePieces[1][0][0].getColor(1) != 'y')) {
                    spinLayerRight90AlongZ(z, false);
                }

                if (cubePieces[0][1][1].getColor(0) == bottomColor) {
                    return 0;
                }
                else if (cubePieces[2][1][1].getColor(0) == bottomColor) {
                    return 2;
                }
            }
        }
    }

    // no non-yellow edge piece in bottom layer

    turnCubeYellowTop();
    counter = 0;
    while (isEdgePieceCorrect(2, 0) && counter < 4) {
        spinRight90AlongZ();
        counter++;
    }
    if (cubePieces[2][0][1].edgePieceContainsColor('y') == false) {
        spinLayerUp90AlongX(2, false);
        spinLayerLeft90AlongZ(2, false);
        spinLayerDown90AlongX(2, false);
        spinDown90AlongX();
        spinDown90AlongX();
        spinRight90AlongZ();
        insertNextWhiteCornerPiece(2);
    }
    return -1;
}

// inserts the next regular edge piece in second layer, starting with the spin of bottom layer away from awayFrom x coordinate
void AlgoCube::insertNextEdgePiece(unsigned awayFrom) {
    if (awayFrom == 0) {
        spinLayerLeft90AlongZ(0, false);
        spinLayerDown90AlongX(0, false);
        spinLayerRight90AlongZ(0, false);
        spinLayerUp90AlongX(0, false);
        spinLeft90AlongZ();
        insertNextWhiteCornerPiece(2);
    }
    else if (awayFrom == 2) {
        spinLayerRight90AlongZ(0, false);
        spinLayerDown90AlongX(2, false);
        spinLayerLeft90AlongZ(0, false);
        spinLayerUp90AlongX(2, false);
        spinRight90AlongZ();
        insertNextWhiteCornerPiece(0);
    }
}

void AlgoCube::turnCubeColorFront(char color) {
    turnCubeWhiteTop();

    while (cubePieces[1][0][1].getColor(0) != color) {
        spinRight90AlongZ();
    }
}

bool AlgoCube::isSecondLayerSolved() {
    turnCubeWhiteTop();

    if (isFirstLayerSolved()) {
        if (cubePieces[0][0][1].getColor(0) == cubePieces[1][0][1].getColor(0) && cubePieces[0][0][1].getColor(1) == cubePieces[0][1][1].getColor(0) &&
        cubePieces[2][0][1].getColor(0) == cubePieces[1][0][1].getColor(0) && cubePieces[2][0][1].getColor(1) == cubePieces[2][1][1].getColor(0) &&
        cubePieces[0][2][1].getColor(0) == cubePieces[1][2][1].getColor(0) && cubePieces[0][2][1].getColor(1) == cubePieces[0][1][1].getColor(0) &&
        cubePieces[2][2][1].getColor(0) == cubePieces[1][2][1].getColor(0) && cubePieces[2][2][1].getColor(1) == cubePieces[2][1][1].getColor(0))
            return true;
    }
    return false;
}

bool AlgoCube::isEdgePieceCorrect(unsigned x, unsigned y) {
    if (y == 0) {
        if (x == 0) {
            return cubePieces[0][0][1].getColor(0) == cubePieces[1][0][1].getColor(0) && cubePieces[0][0][1].getColor(1) == cubePieces[0][1][1].getColor(0);
        }
        else if (x == 2) {
            return cubePieces[2][0][1].getColor(0) == cubePieces[1][0][1].getColor(0) && cubePieces[2][0][1].getColor(1) == cubePieces[2][1][1].getColor(0);
        }
    }
    else if (y == 2) {
        if (x == 0) {
            return cubePieces[0][2][1].getColor(0) == cubePieces[1][2][1].getColor(0) && cubePieces[0][2][1].getColor(1) == cubePieces[0][1][1].getColor(0);
        }
        else if (x == 2) {
            return cubePieces[2][2][1].getColor(0) == cubePieces[1][2][1].getColor(0) && cubePieces[2][2][1].getColor(1) == cubePieces[2][1][1].getColor(0);
        }
    }
    return false;
}

/****************************************************
 * a little section for movement function defs that *
 * are shorter than the previously used             *
 * **************************************************
*/

void AlgoCube::r() {
    spinLayerUp90AlongX(2, false);
}

void AlgoCube::r_() {
    spinLayerDown90AlongX(2, false);
}

void AlgoCube::u() {
    spinLayerRight90AlongZ(2, false);
}

void AlgoCube::u_() {
    spinLayerLeft90AlongZ(2, false);
}

void AlgoCube::l() {
    spinLayerDown90AlongX(0, false);
}

void AlgoCube::l_() {
    spinLayerUp90AlongX(0, false);
}

void AlgoCube::f() {
    spinLayerRight90AlongY(0, false);
}

void AlgoCube::f_() {
    spinLayerLeft90AlongY(0, false);
}



/***    	       SOLVE THIRD LAYER             ***/

void AlgoCube::solveThirdLayer() {

    if (!isSecondLayerSolved()) {
        solveSecondLayer();
    }

    turnCubeYellowTop();
    buildYellowCross();
    connectEdges();
    bringCornersIntoCorrectPosition();
    bringCornersIntoCorrectOrientation();

    if (!isThirdLayerSolved()) {
        solveThirdLayer();
    }
    else {
        cout << "solved third layer" << endl;
    }
}

void AlgoCube::buildYellowCross() {

    while (!isColorCrossOnTopNoSecondary('y')) {
        prepareNextStepYellowCross();
        f();
        r();
        u();
        r_();
        u_();
        f_();
    }
    cout << "yellow cross built" << endl;
}

void AlgoCube::prepareNextStepYellowCross() {
    unsigned x, y, z;
    unsigned counter;

    if (isColorCrossOnTopNoSecondary('y')) {
        return;
    }

    z = 2;
    counter = 0;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && cubePieces[x][y][z].isColorOnTopOfEdgePiece('y', x))
                counter++;
        }
    }

    if (counter == 0) { // no edge piece yellow on top -> only yellow surface -> cube orientation doesnt matter
        return;
    }
    else if (counter == 2) {
        while (cubePieces[1][0][2].isColorOnTopOfEdgePiece('y', 1) == false) {
            spinRight90AlongZ();
        }
        if (cubePieces[1][0][2].isColorOnTopOfEdgePiece('y', 1)) {
            if (cubePieces[1][2][2].isColorOnTopOfEdgePiece('y', 1)) { // yellow line (vertically oriented)
                spinRight90AlongZ();
                // oriented horizontally now
                return;
            }
            else if (cubePieces[0][1][2].isColorOnTopOfEdgePiece('y', 0)) { // yellow L to the left edge
                spinLeft90AlongZ();
                // oriented to the right now
                return;
            }
            else if (cubePieces[2][1][2].isColorOnTopOfEdgePiece('y', 2)) { // yellow L to the right edge
                return;
            }
        }
    }
}

// perform rotations so that all secondary colors of yellow edge pieces match surfaces below
void AlgoCube::connectEdges() {

    while (!isColorCrossOnTop('y')) { // secondary colors need to match as well
        while (edgesConnected() == 0) { // no edges right
            spinLayerRight90AlongZ(2, false);
        }
        if (edgesConnected() == 1) {
            // while not both bottom and top edge piece match surfaces, spin cube right
            while (!(edgePieceSecondaryMatchesSurface(1, 0, 2) && edgePieceSecondaryMatchesSurface(1, 2, 2))) {
                spinRight90AlongZ();
            }
            r();
            u();
            u();
            r_();
            u_();
            r();
            u_();
            r_();
        }
        if (edgesConnected() == 2) {  // two edges next each other correct
            // while not both bottom and right edge pieces dont match surfaces, spin cube right
            while (!(edgePieceSecondaryMatchesSurface(1, 0, 2) == false && edgePieceSecondaryMatchesSurface(2, 1, 2) == false)) {
                spinRight90AlongZ();
            }
            r();
            u();
            u();
            r_();
            u_();
            r();
            u_();
            r_();
            u_();
        }
    }
    cout << "connected edges" << endl;
}

// returns 0 if no edges are connected with surface below, 1 if two opposite edges, 2 if two besided edges, 4 if all edges are connected with surfaces below
unsigned AlgoCube::edgesConnected() {
    unsigned x, y, z;
    unsigned counter;

    z = 2;
    counter = 0;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isEdgePiece() && edgePieceSecondaryMatchesSurface(x, y, z)) {
                counter++;
            }
        }
    }
    if (counter < 2) {
        return 0;
    }
    else if (counter == 2) {
        while (edgePieceSecondaryMatchesSurface(1, 0, 2) == false) {
            spinRight90AlongZ();
        }
        if (edgePieceSecondaryMatchesSurface(1, 0, 2)) {
            if (edgePieceSecondaryMatchesSurface(1, 2, 2)) {
                return 1;
            }
            else if (edgePieceSecondaryMatchesSurface(0, 1, 2) || edgePieceSecondaryMatchesSurface(2, 1, 2)) {
                return 2;
            }
        }
    }

    // all edges connected
    return 4;
}

void AlgoCube::bringCornersIntoCorrectPosition() {
    unsigned nrCornersCorrectPos;

    nrCornersCorrectPos = getNrCornerPiecesInCorrectPosition();
    while (nrCornersCorrectPos < 4) {
        if (nrCornersCorrectPos == 1) {
            while (!isCornerPieceInCorrectPosition(2, 0, 'y')) {
                spinRight90AlongZ();
            }
        }
        l_();
        u();
        r();
        u_();
        l();
        u();
        r_();
        u_();
        nrCornersCorrectPos = getNrCornerPiecesInCorrectPosition();
    }
    cout << "positioned corners in third layer correctly" << endl;
}

// can only return 0, 1 or 4
unsigned AlgoCube::getNrCornerPiecesInCorrectPosition() {
    unsigned x, y, z;
    unsigned counter;

    z = 2;
    counter = 0;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece() && isCornerPieceInCorrectPosition(x, y, 'y')) {
                counter++;
            }
        }
    }
    return counter;
}

void AlgoCube::bringCornersIntoCorrectOrientation() {
    unsigned nrCornersCorrect;

    nrCornersCorrect = getNrCornerPiecesInCorrectOrientation();
    while (getNrCornerPiecesInCorrectOrientation() < 4) {
        if (nrCornersCorrect == 1) {
            while (!isCornerPieceCorrect(0, 0, 'y')) { // correct one should be in bottom left
                spinRight90AlongZ();
            }
        }
        if (nrCornersCorrect == 2) {
            while (!(isCornerPieceCorrect(0, 0, 'y') && isCornerPieceCorrect(2, 0, 'y') == false)) {
                spinRight90AlongZ();
            }
        }

        r();
        u();
        u();
        r_();
        u_();
        r();
        u_();
        r_();

        l_();
        u();
        u();
        l();
        u();
        l_();
        u();
        l();

        nrCornersCorrect = getNrCornerPiecesInCorrectOrientation();
    }
    cout << "orientated corners in third layer correctly" << endl;
}

// can only return 0, 1, 2, 4
unsigned AlgoCube::getNrCornerPiecesInCorrectOrientation() {
    unsigned x, y, z;
    unsigned counter;

    z = 2;
    counter = 0;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece() && isCornerPieceCorrect(x, y, 'y')) {
                counter++;
            }
        }
    }
    return counter;
}

// returns true if the corner piece is within the layer of passed @param color, and contains neighbored colors of surfaces as well, else false
bool AlgoCube::isCornerPieceInCorrectPosition(unsigned x, unsigned y, char color) {
    char surfaceColorFrontBack;
    char surfaceColorLeftRight;
    unsigned z;

    z = 2;
    if (cubePieces[x][y][z].getPositionOfColor(color) != -1) {
        surfaceColorFrontBack = cubePieces[1][y][1].getColor(0);
        surfaceColorLeftRight = cubePieces[x][1][1].getColor(0);
        if (cubePieces[x][y][z].getPositionOfColor(surfaceColorFrontBack) != -1 && cubePieces[x][y][z].getPositionOfColor(surfaceColorLeftRight) != -1) {
            return true;
        }
    }
    return false;
}

// returns true if yellow cross on top (criterias like in isColorCrossOnTop() concerning secondary colors not considered)
bool AlgoCube::isColorCrossOnTopNoSecondary(char color) {
    if (cubePieces[1][0][2].isColorOnTopOfEdgePiece(color, 1) &&
    cubePieces[0][1][2].isColorOnTopOfEdgePiece(color, 0) &&
    cubePieces[2][1][2].isColorOnTopOfEdgePiece(color, 2) &&
    cubePieces[1][2][2].isColorOnTopOfEdgePiece(color, 1)) {
        return true;
    }
    return false;
}

bool AlgoCube::isThirdLayerSolved() {
    unsigned x, y, z;

    turnCubeYellowTop();

    z = 2;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (cubePieces[x][y][z].isCornerPiece() && isCornerPieceCorrect(x, y, 'y') == false) {
                return false;
            }
        }
    }
    if (!isColorCrossOnTop('y')) {
        return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////

void AlgoCube::solveRubiksCube() {
    solveFirstLayer();
    solveSecondLayer();
    solveThirdLayer();

    cout << "-- Rubik's Cube solved successfully --" << endl;

}

/***    	           BUILD CUBE                ***/

void AlgoCube::initCube() {
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
    CubePiece c122 = CubePiece("oy");
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

    setPieces(testCube);

}

void AlgoCube::createRandomCube() {
    srand(time(NULL));
    unsigned randomLoops = rand() % 40 + 12; // at least 12 moves
    unsigned randomMove;
    unsigned randomNrRotations;
    unsigned randomLayer;
    unsigned i, p;

    cout << "started to rotate cube randomly...\n.\n." << endl;

    for (i = 0; i < randomLoops; i++) {
        randomMove = rand() % 6;
        randomNrRotations = rand() % 3 + 1;
        randomLayer = rand() % 3;
        switch(randomMove) {
            case 0:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerUp90AlongX(randomLayer, true, 1);
                cout << "spinned layer up 90 along x, layer: " << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
            case 1:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerDown90AlongX(randomLayer, true, 1);
                cout << "spinned layer down 90 along x, layer: " << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
            case 2:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerRight90AlongY(randomLayer, true, 1);
                cout << "spinned layer right 90 along y, layer: " << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
            case 3:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerLeft90AlongY(randomLayer, true, 1);
                cout << "spinned layer left 90 along y, layer:" << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
            case 4:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerRight90AlongZ(randomLayer, true, 1);
                cout << "spinned layer right 90 along z, layer: " << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
            case 5:
                for (p = 0; p < randomNrRotations; p++)
                    spinLayerLeft90AlongZ(randomLayer, true, 1);
                cout << "spinned layer left 90 along z, layer: " << randomLayer << ", " << randomNrRotations << " times" << endl;
                break;
        }
    }
    cout << ".\n.\n...finished rotating cube randomly" << endl;

}


/**********************************************************************
**                    GLOBAL FUNCTION DEFINITIONS                    **
**********************************************************************/



// testing function
int test() {

    AlgoCube cube = AlgoCube();

    cube.initCube();
    cube.createRandomCube();

    // cube.printWholeCube();

    cube.solveRubiksCube();
    cube.printWholeCube();

    cout << "Nr Moves: " << cube.getMoves().size() << endl;

    for (unsigned i = 0; i < cube.getRandomizeCubeMoves().size(); i++) {
        cout << "Randomize " << i << ": " << cube.getRandomizeCubeMoves().at(i) << endl;
    }

    for (unsigned i = 0; i < cube.getMoves().size(); i++) {
        cout << "Move " << i << ": " << cube.getMoves().at(i) << endl;
    }

    return 0;
}

int main() {
    test();
}
