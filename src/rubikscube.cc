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
**                         DEFINITION SCOPE                          **
**********************************************************************/

SubCube::SubCube(){}

SubCube::SubCube(string colorstring) {
    unsigned i;
    for (i = 0; i < colorstring.size(); i++) {
        colors.push_back(colorstring.at(i));
    }
}

void SubCube::setSubCube(string colorstring) {
    colors.clear();
    unsigned i;
    for (i = 0; i < colorstring.size(); i++) {
        colors.push_back(colorstring.at(i));
    }
}

vector<char> SubCube::getSubCube() {
    return colors;
}

void SubCube::print() {
    string out = "";
    unsigned i;
    for (i = 0; i < colors.size(); i++) {
        if (i == colors.size() -1) {
            cout << out << colors[i] << "\n";
            return;
        }
        out = out + colors[i] + ", ";
    }
}

Cube::Cube(){}

Cube::Cube(vector<SubCube> subCubes) {
    unsigned i, x, y, z;
    i = 0;

    for (z = 0; z < 3; z++) {
        for (y = 0; y < 3; y++) {
            for (x = 0; x < 3; x++) {
                if (i < 27) {
                    cubes[x][y][z] = subCubes[i];
                    i++;
                }
            }
        }
    }
}

void Cube::printFirstLayer() {
    unsigned i;
    string out = "";

    for (i = 0; i < 3; i++) {
        cubes[i][0][0].print();
        cout << "\n";
    }
}

// function writes new subCube on cubes[x][y][z] and returns old one
SubCube Cube::setNewPart(unsigned x, unsigned y, unsigned z, SubCube subCube) {
    SubCube tmp = cubes[x][y][z];
    cubes[x][y][z] = subCube;
    return tmp;
}

Cube turn180Vert(Cube cube) {
    return cube;
}

int test() {
    // testing
    vector<SubCube> testCube;

    // first layer:
    SubCube c000 = SubCube("bwo");
    SubCube c100 = SubCube("yb");
    SubCube c200 = SubCube("ybr");
    SubCube c010 = SubCube("gw");
    SubCube c110 = SubCube("w");
    SubCube c210 = SubCube("rb");
    SubCube c020 = SubCube("rwg");
    SubCube c120 = SubCube("gr");
    SubCube c220 = SubCube("brw");

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
    SubCube c001 = SubCube("ob");
    SubCube c101 = SubCube("r");
    SubCube c201 = SubCube("wo");
    SubCube c011 = SubCube("b");
    SubCube c111 = SubCube("-");
    SubCube c211 = SubCube("g");
    SubCube c021 = SubCube("wb");
    SubCube c121 = SubCube("o");
    SubCube c221 = SubCube("ry");

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
    SubCube c002 = SubCube("gyr");
    SubCube c102 = SubCube("yg");
    SubCube c202 = SubCube("oyb");
    SubCube c012 = SubCube("wr");
    SubCube c112 = SubCube("y");
    SubCube c212 = SubCube("oy");
    SubCube c022 = SubCube("wgo");
    SubCube c122 = SubCube("og");
    SubCube c222 = SubCube("goy");

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

    cube.printFirstLayer();

    return 0;
}

int main() {
    test();
}
