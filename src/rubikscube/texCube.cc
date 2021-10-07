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

    return addCube(vtx, this->position, this->depth);
}

std::array<GLfloat, 5 * 36> TexCube::addCube(std::array<GLfloat, 5 * 36> vtx, int position, GLfloat depth)
{

    int elementsPerCube = 36 * 5;

    for (int i = 0; i + 4 < elementsPerCube; i += 5)
    {
        vtx[i + 2] = vtx[i + 2] + depth;

        if (position == this->LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
            } else if (depth == -2.1f) {
                vtx[i + 3] = this->initCube[i + 3];
                vtx[i + 4] = this->initCube[i + 4];
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
            } else if (depth == -4.2f) {
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
            }
        }

        if (position == this->RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -2.1f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
            }
        }

        if (position == this->TOP)
        {
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
            } else if (depth == -2.1f) {
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            }
        }

        if (position == this->BOTTOM)
        {
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                vtx[i + 4] = this->initCube[i + 4];
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            } else if (depth == -2.1f) {
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
            }
        }

        if (position == this->TOP_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4];
                }
            } else if (depth == -2.1f) {
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            }
        }

        if (position == this->TOP_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
            } else if (depth == -2.1f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f; 
                }
                if(i > 29 && i < 60) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            }
        }

        if (position == this->BOTTOM_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4];
                }
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            } else if (depth == -2.1f) {
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 119 && i < 150 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4];
                }
            }
        }

        if (position == this->BOTTOM_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;
            if(depth == 0) {
                if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4];
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 2.0f/6.0f;
                }
            } else if (depth == -2.1f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
            } else if (depth == -4.2f) {
                if(i > 149 && i < 180) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3];
                    vtx[i + 4] = this->initCube[i + 4]; 
                }
                if(i >= 0 && i < 30) {
                    vtx[i + 3] = this->initCube[i + 3] + 2.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4];
                }
            }
        } else if (position == this->MIDDLE) {
          if (depth == 0) {
              if(i > 59 && i < 90) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f;
                }
          } else if (depth == -2.1f) {
               vtx[i + 3] = this->initCube[i + 3];
               vtx[i + 4] = this->initCube[i + 4];
          } else if (depth == -4.2f) {
                if(i > 89 && i < 120 ) {
                    vtx[i + 3] = this->initCube[i + 3] + 1.0f/9.0f;
                    vtx[i + 4] = this->initCube[i + 4] + 1.0f/6.0f; 
                }
            }
       }
    }

    return vtx;
}