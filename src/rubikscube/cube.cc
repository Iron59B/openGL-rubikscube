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

#include "cube.h"

std::array<GLfloat, 6 * 36> Cube::createCubes(int position, char c1, char c2, char c3)
{
    std::array<GLfloat, 6 * 36> vtx;
    for (int i = 0; i < 6 * 36; i++)
    {
        vtx[i] = this->initCube[i];
    }

    if (this->position == this->MIDDLE)
    {
        // printf("START OF INIT CUBE \n");
        return vtx;
    }
    else
    {
        // std::copy(std::begin(this->initCube), std::end(this->initCube), std::begin(vtx));
        return addCube(vtx, position, c1, c2, c3);
    }
}

std::array<GLfloat, 6 * 36> Cube::addCube(std::array<GLfloat, 6 * 36> vtx, int position, char c1, char c2, char c3)
{
    GLfloat depth;

    // printf("Pointer: %u\n", &vtx);
    // std::cout << &vtx << std::endl;

    // std::copy(std::begin(this->initCube), std::end(this->initCube), std::begin(vtx));
    // printf("Pointer before alg: %u\n", *vtx);
    // printf("Pointer during alg: %u\n", *vtx);

    int elementsPerCube = 36 * 6;

    //left cube
    // printf("START OF CUBE \n");
    /*
      Create one cube by iteration with 6 iterations:
      First iteration -> bottom
      Second iteration -> top
      Third iteration -> front
      Fourth iteration -> back
      Fifth iteration -> left
      Sixth iteration -> right
    */

    if (position < 9)
      depth = 0.0f;
    else if (position < 18)
      depth = -2.1f;
    else
      depth = -4.2f;

    for (int i = 0; i + 5 < elementsPerCube; i += 6)
    {
        vtx[i + 2] = vtx[i + 2] + depth;

        if (position % 9 == this->LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }

        }

        if (position % 9  == this->RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->TOP)
        {
            vtx[i + 1] = vtx[i + 1] + 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->BOTTOM)
        {
            vtx[i + 1] = vtx[i + 1] - 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->MIDDLE)
        {
            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->TOP_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->TOP_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] + 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 1*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->BOTTOM_LEFT)
        {
            vtx[i] = vtx[i] - 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 4*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        if (position % 9 == this->BOTTOM_RIGHT)
        {
            vtx[i] = vtx[i] + 2.1f;
            vtx[i + 1] = vtx[i + 1] - 2.1f;

            if (depth == 0.0f) {
              if (i == 2*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -2.1f) {
              if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            } else if (depth == -4.2f) {
              if (i == 3*6) {
                vtx = addColor(getColor(c1), vtx, i+3);
              } else if (i == 0*6) {
                vtx = addColor(getColor(c2), vtx, i+3);
              } else if (i == 5*6) {
                vtx = addColor(getColor(c3), vtx, i+3);
              } else {
                vtx = addColor(this->BLACK, vtx, i+3);
              }
            }
        }

        // if (i == 0) {
        //   vtx = addColor(this->BLUE, vtx, i+3);
        // } else if (i == 6) {
        //   vtx = addColor(this->GREEN, vtx, i+3);
        // } else if (i == 2*6) {
        //   vtx = addColor(this->RED, vtx, i+3);
        // } else if (i == 3*6) {
        //   vtx = addColor(this->ORANGE, vtx, i+3);
        // } else if (i == 4*6) {
        //   vtx = addColor(this->WHITE, vtx, i+3);
        // } else if (i == 5*6) {
        //   vtx = addColor(this->YELLOW, vtx, i+3);
        // }

        // vtx[i + 3] = this->initCube[i + 3];
        // vtx[i + 4] = this->initCube[i + 4];
        // vtx[i + 5] = this->initCube[i + 5];
        //printf("%f, %f, %f, %f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2], vtx[i+3], vtx[i+4], vtx[i+5]);
        // printf("x: %f ", vtx[i]);
        // printf("y: %f ", vtx[i+1]);
        // printf("z: %f \n", vtx[i+2]);
        // if(i+5 == elementsPerCube - 1) {
        //     printf("END OF CUBE \n");
        // }
    }

    // printf("Pointer after alg: %u\n", *vtx);
    return vtx;
}

std::array<GLfloat, 6*36> Cube::addColor(int color, std::array<GLfloat, 6*36> vtx, int i) {
  if(color == this->YELLOW) {
    vtx[i] = 1.0f;
    vtx[i+1] = 1.0f;
    vtx[i+2] = 0.0f;
  } else if(color == this->RED) {
    vtx[i] = 1.0f;
    vtx[i+1] = 0.0f;
    vtx[i+2] = 0.0f;
  } else if(color == this->BLUE) {
    vtx[i] = 0.0f;
    vtx[i+1] = 0.0f;
    vtx[i+2] = 1.0f;
  } else if(color == this->GREEN) {
    vtx[i] = 0.0f;
    vtx[i+1] = 1.0f;
    vtx[i+2] = 0.0f;
  } else if(color == this->ORANGE) {
    vtx[i] = 1.0f;
    vtx[i+1] = 0.5f;
    vtx[i+2] = 0.0f;
  } else if(color == this->WHITE) {
    vtx[i] = 1.0f;
    vtx[i+1] = 1.0f;
    vtx[i+2] = 1.0f;
  } else if(color == this->BLACK) {
    vtx[i] = 0.1f;
    vtx[i+1] = 0.1f;
    vtx[i+2] = 0.1f;
  }

  return vtx;
}

int Cube::getColor(char color) {
  int x = -1;

  if (color == 'r')
    x = this->RED;
  if (color == 'w')
    x = this->WHITE;
  if (color == 'g')
    x = this->GREEN;
  if (color == 'y')
    x = this->YELLOW;
  if (color == 'b')
    x = this->BLUE;
  if (color == 'o')
    x = this->ORANGE;

  return x;
}
