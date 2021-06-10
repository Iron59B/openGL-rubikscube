#ifndef cube_h
#define cube_h
/* standard includes */
#include<stdio.h>
#include<stdlib.h>

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

class Cube {
public:
    Cube(){

    };
    Cube(int position, GLfloat depth){
        this->position = position;
        this->depth = depth; 
    };
    GLfloat* createCubes();

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

    GLfloat* addCube(int position, GLfloat depth);
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