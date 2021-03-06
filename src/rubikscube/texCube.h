#ifndef texCube_h
#define texCube_h
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

class TexCube {
public:
    TexCube(){ };
    TexCube(int position, GLfloat depth){
        this->position = position;
        this->depth = depth;
    };
    array<GLfloat,5*36> createCubes();
    
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

    array<GLfloat,5*36> addCube(array<GLfloat,5*36> vtx, int position, GLfloat depth);
    GLfloat initCube[5*36] = {
        -1.0f,-1.0f,-1.0f,   6.0f/9.0f, 3.0f/6.0f, /* 0-29 bottom: white */
         1.0f,-1.0f,-1.0f,   7.0f/9.0f, 3.0f/6.0f,
        -1.0f,-1.0f, 1.0f,   6.0f/9.0f, 4.0f/6.0f,
         1.0f,-1.0f,-1.0f,   7.0f/9.0f, 3.0f/6.0f,
         1.0f,-1.0f, 1.0f,   7.0f/9.0f, 4.0f/6.0f,
        -1.0f,-1.0f, 1.0f,   6.0f/9.0f, 4.0f/6.0f,
        -1.0f, 1.0f,-1.0f,   6.0f/9.0f, 1.0f/6.0f, /* 30-59 top: yellow */
        -1.0f, 1.0f, 1.0f,   6.0f/9.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   7.0f/9.0f, 1.0f/6.0f,
         1.0f, 1.0f,-1.0f,   7.0f/9.0f, 1.0f/6.0f,
        -1.0f, 1.0f, 1.0f,   6.0f/9.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   7.0f/9.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 0.0f, /* 60-89 front: red */
         1.0f,-1.0f, 1.0f,   1.0f/9.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f/6.0f,
         1.0f,-1.0f, 1.0f,   1.0f/9.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   1.0f/9.0f, 1.0f/6.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f/6.0f,
        -1.0f,-1.0f,-1.0f,   1.0f/9.0f, 3.0f/6.0f, /* 90-119 back: orange */
        -1.0f, 1.0f,-1.0f,   1.0f/9.0f, 4.0f/6.0f,
         1.0f,-1.0f,-1.0f,   0.0f, 3.0f/6.0f,
         1.0f,-1.0f,-1.0f,   0.0f, 3.0f/6.0f,
        -1.0f, 1.0f,-1.0f,   1.0f/9.0f, 4.0f/6.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 4.0f/6.0f,
        -1.0f,-1.0f, 1.0f,   4.0f/9.0f, 0.0f, /* 120-149 left: blue */
        -1.0f, 1.0f,-1.0f,   3.0f/9.0f, 1.0f/6.0f,
        -1.0f,-1.0f,-1.0f,   3.0f/9.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   4.0f/9.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   4.0f/9.0f, 1.0f/6.0f,
        -1.0f, 1.0f,-1.0f,   3.0f/9.0f, 1.0f/6.0f,
         1.0f,-1.0f, 1.0f,   3.0f/9.0f, 3.0f/6.0f, /* 150-179 right: green */
         1.0f,-1.0f,-1.0f,   4.0f/9.0f, 3.0f/6.0f,
         1.0f, 1.0f,-1.0f,   4.0f/9.0f, 4.0f/6.0f,
         1.0f,-1.0f, 1.0f,   3.0f/9.0f, 3.0f/6.0f,
         1.0f, 1.0f,-1.0f,   4.0f/9.0f, 4.0f/6.0f,
         1.0f, 1.0f, 1.0f,   3.0f/9.0f, 4.0f/6.0f

    };
};


#endif /* cube_h */
