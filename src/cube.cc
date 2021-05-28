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

#include "cube.h"


GLfloat* Cube::createCubes()
{
    GLfloat vtx[26*7*36] = {
        /* first, intial cube */
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
    
    addingFrontRubik(vtx);
    addingMiddleRubik(vtx);
    addingBackRubik(vtx);
    
    return vtx;
}

GLfloat* Cube::addingBackRubik(GLfloat* vtx)
{
    int trianglesInCube = 36;
    int elementsPerCube = 36*6;
    int numberOfCubes = 26;
    int cubeNum = 17;
    
    //back cube
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.5f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 1.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 4.2f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%u: %f, %f, %f, %f, %f, %f \n", i, vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    return vtx;
}


GLfloat* Cube::addingMiddleRubik(GLfloat* vtx)
{
    int trianglesInCube = 36;
    int elementsPerCube = 36*6;
    int numberOfCubes = 26;
    int cubeNum = 9;
    
    //left cube: red
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //right cube: green
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower cube: blue
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 1.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 1.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2] - 2.1f;
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 1.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    return vtx;
}

GLfloat* Cube::addingFrontRubik(GLfloat* vtx)
{
    int trianglesInCube = 36;
    int elementsPerCube = 36*6;
    int numberOfCubes = 26;
    int cubeNum = 1;
    
    //left cube
    for(int i = elementsPerCube; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube + 1];
        vtx[i+2] = vtx[i-elementsPerCube + 2];
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1];
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 0.5f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //upper right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] + 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum];
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 0.0f;
        vtx[i+5] = 1.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower left cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] - 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 1.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    //lower right cube
    cubeNum++;
    for(int i = elementsPerCube*cubeNum; i < elementsPerCube*(cubeNum+1); i+=6) {
        vtx[i] = vtx[i-elementsPerCube*cubeNum] + 2.1f;
        vtx[i+1] = vtx[i-elementsPerCube*cubeNum + 1] - 2.1f;
        vtx[i+2] = vtx[i-elementsPerCube*cubeNum + 2];
        
        vtx[i+3] = 0.0f;
        vtx[i+4] = 1.0f;
        vtx[i+5] = 0.0f;
        
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }
    
    return vtx;
}


