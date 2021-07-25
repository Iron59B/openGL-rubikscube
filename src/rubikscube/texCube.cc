/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <iostream>
#include <vector>

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

#include "texCube.h"

std::array<GLfloat, 5 * 36> TexCube::createCubes()
{
    std::array<GLfloat, 5 * 36> vtx;
    for (int i = 0; i < 5 * 36; i++)
    {
        vtx[i] = this->initCube[i];
    }

    if (this->position == this->MIDDLE && (this->depth == 0))
    {
        // printf("START OF INIT CUBE \n");
      return vtx;
    }
    else
    {
        // std::copy(std::begin(this->initCube), std::end(this->initCube), std::begin(vtx));
        return addCube(vtx, this->position, this->depth);
    }
}

std::array<GLfloat, 5 * 36> TexCube::addCube(std::array<GLfloat, 5 * 36> vtx, int position, GLfloat depth)
{

    // printf("Pointer: %u\n", &vtx);
    // std::cout << &vtx << std::endl;

    // std::copy(std::begin(this->initCube), std::end(this->initCube), std::begin(vtx));
    // printf("Pointer before alg: %u\n", *vtx);
    // printf("Pointer during alg: %u\n", *vtx);

    int elementsPerCube = 36 * 5;

    //left cube
    // printf("START OF CUBE \n");
    for (int i = 0; i + 4 < elementsPerCube; i += 5)
    {
        vtx[i + 2] = vtx[i + 2] + depth;

        if (position == this->LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 143 && i < 180)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i > 143 && i < 180)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 143 && i < 180)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 179 && i < 216)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i > 179 && i < 216)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 179 && i < 216)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->TOP)
        {
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 35 && i < 72)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i > 35 && i < 72)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 35 && i < 72)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->BOTTOM)
        {
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i < 36)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i < 36)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i < 36)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->TOP_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 35 && i < 72) || (i > 143 && i < 180)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i > 35 && i < 72)  || (i > 143 && i < 180)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 143 && i < 180) || (i > 35 && i < 72)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->TOP_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 179 && i < 216) || (i > 35 && i < 72)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i > 35 && i < 72) || (i > 179 && i < 216)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 179 && i < 216) || (i > 35 && i < 72)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->BOTTOM_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 143 && i < 180) || (i < 36)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -2.1f) {
            //   if((i < 36) || (i > 143 && i < 180)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 143 && i < 180) || (i < 36)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        }

        if (position == this->BOTTOM_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            // if(depth == 0) {
            //   if((i > 71 && i < 108) || (i > 179 && i < 216) || (i < 36)) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
            //   }  
            // } else if (depth == -2.1f) {
            //   if((i < 36) || (i > 179 && i < 216)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // } else if (depth == -4.2f) {
            //   if((i > 107 && i < 144) || (i > 179 && i < 216) || (i < 36)) {
            //     vtx[i + 3] = this->initCube[i + 3];
            //     vtx[i + 4] = this->initCube[i + 4];
            //   } 
            // }
        } else if (position == this->MIDDLE) {
        //   if (depth == -2.1f) {
            // for (int i = 0; i < 36*6; i+=6) {
               vtx[i + 3] = this->initCube[i + 3];
               vtx[i + 4] = this->initCube[i + 4];
            //  }
        //   } else if (depth == -4.2f) {
        //    for (int i = 0; i < 36*6; i+=6) {
        //      if ((i < 108 || i > 143)) {
        //        vtx[i + 3] = 0.25f;
        //        vtx[i + 4] = 0.25f;
        //      }
        //    }
        }
       }


        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
        // printf("x: %f ", vtx[i]);
        // printf("y: %f ", vtx[i+1]);
        // printf("z: %f \n", vtx[i+2]);
        // if(i+5 == elementsPerCube - 1) {
        //     printf("END OF CUBE \n");
        // }
    // }

    // printf("Pointer after alg: %u\n", *vtx);
    return vtx;
}