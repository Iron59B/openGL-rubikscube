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
    char getColorChar();
};

class CubePiece {
private:
    /***************************************************************
    ** Each CubePiece consists of maximum 3 colors as there are   **
    ** corners (3), edges (2) and surfaces (1). The color no-     **
    ** tation order of each CubePiece is consistent:              **
    ** CORNERS -- (1) front-back, (2) top-bottom (3) left-right   **
    ** EDGES -- (1) front-back, (2) top-bottom (3) left-right     **
    **       -- consist of only 2 of these 3 orientations         **
    ** SURFACES -- consist of only 1 color at [0]                 **
    ***************************************************************/
    vector<Color> colors;
public:
    CubePiece();
    CubePiece(string colorstring);
    void setColors(string colorstring);
    char getColor(unsigned index);
    void print();
    bool isCornerPiece();
    bool isEdgePiece();
    bool isSurfacePiece();
    void prepareCornerPieceMove90AlongX();
    void prepareEdgePieceMove90AlongX(unsigned z, unsigned x);
    void prepareCornerPieceMove90AlongY();
    void prepareEdgePieceMove90AlongY(unsigned y);
    void prepareCornerPieceMove90AlongZ();
    void prepareEdgePieceMove90AlongZ();

    int getPositionOfColor(char color);
    bool isColorOnTopOfEdgePiece(char color, unsigned x);
    bool edgePieceContainsColor(char color);

};

class Cube {
private:
    //      [x: left->right] [y: front->back] [z: bottom->top]
    CubePiece cubePieces[3][3][3];
public:
    Cube();
    Cube(vector<CubePiece> pieces);
    void createRandomCube();
    void printFirstLayer();
    void printWholeCube();
    void spinUp180AlongX();
    void spinUp90AlongX();
    void spinLayerUp90AlongX(unsigned xLayer);
    void spinDown90AlongX();
    void spinLayerDown90AlongX(unsigned xLayer);
    void spinRight90AlongY();
    void spinLayerRight90AlongY(unsigned yLayer);
    void spinLeft90AlongY();
    void spinLayerLeft90AlongY(unsigned yLayer);
    void spinRight90AlongZ();
    void spinLayerRight90AlongZ(unsigned zLayer);
    void spinLeft90AlongZ();
    void spinLayerLeft90AlongZ(unsigned zLayer);

    bool edgePieceSecondaryMatchesSurface(unsigned x, unsigned y, unsigned z);

    // first layer functions

    void solveFirstLayer();
    void buildWhiteFlowerMiddleLayer();
    void buildWhiteFlowerBottomLayer();
    void buildWhiteFlowerTopLayer();
    void buildWhiteFlower();

    void buildWhiteCross();
    void bringBottomWhiteCornerFront();
    void bringTopWhiteCornerFront();
    void dismantleIncorrectWhiteCornerPiece(unsigned x, unsigned y);
    void insertNextWhiteCornerPiece(unsigned x);
    
    void turnCubeYellowTop();
    void turnCubeWhiteTop();
    int turnCubeUntilWhiteBottomCornerFront();

    int isWhiteBottomCornerFront();
    bool isWhiteBottomCornerLeftBottom();
    bool isWhiteCrossOnBottom();
    bool isColorCrossOnTop(char color);
    bool isWhiteFlowerOnTop();
    bool isFirstLayerSolved();
    bool isCornerPieceCorrect(unsigned x, unsigned y, char color);

    // second layer functions

    void solveSecondLayer();
    int prepareForNextEdgePieceInsertion();
    void insertNextEdgePiece(unsigned awayFrom);
    void turnCubeColorFront(char color);
    bool isSecondLayerSolved();
    bool isEdgePieceCorrect(unsigned x, unsigned y);

    // third layer functions

    void r();
    void r_();
    void u();
    void u_();
    void l();
    void l_();
    void f();
    void f_();

    void solveThirdLayer();
    void solveYellowCross();
    void prepareNextStepYellowCross();
    bool isColorCrossOnTopNoSecondary(char color);
    bool isThirdLayerSolved();
};



#endif