#ifndef cube_h
#define cube_h
/* standard includes */
#include<stdio.h>
#include<stdlib.h>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
#include <SOIL.h>
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
    Cube(int size){
        this->size = size;
    };
    GLfloat* createCubes();

private:
    int size;
    GLfloat* addingFrontRubik(GLfloat* vtx);
    GLfloat* addingMiddleRubik(GLfloat* vtx);
    GLfloat* addingBackRubik(GLfloat* vtx);
};


#endif /* cube_h */
