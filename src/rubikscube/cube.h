#ifndef cube_h
#define cube_h
/* standard includes */
#include<stdio.h>
#include<stdlib.h>
#include<array>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
// #include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include<GLFW/glfw3.h>
/* GLNM is used for handling vector and matrix math */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Cube {
public:
    Cube(){

    };
    Cube(int position, GLfloat depth){
        this->position = position;
        this->depth = depth;
    };
    array<GLfloat,6*36> createCubes();

    // array<GLfloat,6*36> getInitCube() {
    //   return initCube;
    // }

private:
    int position;
    GLfloat depth;
    const int MIDDLE = 0;
    const int LEFT = 1;
    const int RIGHT = 2;
    const int TOP = 3;
    const int BOTTOM = 4;
    const int TOP_LEFT = 5;
    const int TOP_RIGHT = 6;
    const int BOTTOM_LEFT = 7;
    const int BOTTOM_RIGHT = 8;

    const int YELLOW = 0;
    const int RED = 1;
    const int BLUE = 2;
    const int GREEN = 3;
    const int ORANGE = 4;
    const int WHITE = 5;

    array<GLfloat,6*36> addCube(array<GLfloat,6*36> vtx, int position, GLfloat depth);
    array<GLfloat,6*36> addColor(int color, std::array<GLfloat, 6*36> vtx, int i);
    GLfloat initCube[6*36] = {
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f, 1.0f, /* bottom: blue */
         1.0f,-1.0f,-1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f, /* top: green */
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f, /* front: red */
         1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   1.0f, 0.5f, 0.0f, /* back: orange */
        -1.0f, 1.0f,-1.0f,   1.0f, 0.5f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.5f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.5f, 0.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.5f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.5f, 0.0f,
        -1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 1.0f, /* left: white */
        -1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,   1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 0.0f, /* right: yellow */
         1.0f,-1.0f,-1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,
    };
};


#endif /* cube_h */
