/* standard includes */
#include <stdio.h>
#include <stdlib.h>

/* We use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* SOIL is used for loading (texture) images */
// #include <SOIL.h>
/* GLFW is used for creating and manipulating graphics windows */
#include <GLFW/glfw3.h>
/* GLNM is used for handling vector and matrix math */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.h"

GLfloat *Cube::createCubes()
{
    if (this->position == this->MIDDLE && this->depth == 0)
        return this->initCube;
    else
    {
        return addCube(this->position, this->depth);
    }
}

GLfloat *Cube::addCube(int position, GLfloat depth)
{
    static GLfloat *vtx = this->initCube;
    int elementsPerCube = 36 * 6;
    const int pos = position;

    //left cube
    for (int i = 0; i < elementsPerCube; i += 6)
    {
        vtx[i + 2] = vtx[i + 2] + depth;

        if (position == this->LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
        }
        if (position == this->RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
        }
        if (position == this->TOP)
        {
            vtx[i + 1] = vtx[i + 1] + 2.1f;
        }
        if (position == this->BOTTOM)
        {
            vtx[i + 1] = vtx[i + 1] - 2.1f;
        }
        if (position == this->TOP_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
        }
        if (position == this->TOP_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
        }
        if (position == this->BOTTOM_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
        }
        if (position == this->BOTTOM_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
        }

        vtx[i + 3] = this->initCube[i + 3];
        vtx[i + 4] = this->initCube[i + 4];
        vtx[i + 5] = this->initCube[i + 5];
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
    }

    return vtx;
}
