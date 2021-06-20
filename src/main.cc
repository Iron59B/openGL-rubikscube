/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

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

#include "rubikscube/cube.h"
#include "rubikscube/rubikscube.h"

#define GLSL(src) "#version 330 core\n" #src
#define GLM_FORCE_RADIANS

using namespace std;

int numCalculated = 0;
int MIDDLE = 0; //+0 -> Front, +9 -> Middle, +18 -> Back
int LEFT = 1;
int RIGHT = 2;
int TOP = 3;
int BOTTOM = 4;
int TOP_LEFT = 5;
int TOP_RIGHT = 6;
int BOTTOM_LEFT = 7;
int BOTTOM_RIGHT = 8;

static int MIDDLE_MIDDLE = MIDDLE+9; //+0 -> Front, +9 -> Middle, +18 -> Back
static int LEFT_MIDDLE = LEFT+9;
static int RIGHT_MIDDLE = RIGHT+9;
static int TOP_MIDDLE = TOP+9;
static int BOTTOM_MIDDLE = BOTTOM+9;
static int TOP_LEFT_MIDDLE = TOP_LEFT+9;
static int TOP_RIGHT_MIDDLE = TOP_RIGHT+9;
static int BOTTOM_LEFT_MIDDLE = BOTTOM_LEFT+9;
static int BOTTOM_RIGHT_MIDDLE = BOTTOM_RIGHT+9;

static int MIDDLE_BACK = MIDDLE+18; //+0 -> Front, +9 -> Middle, +18 -> Back
static int LEFT_BACK = LEFT+18;
static int RIGHT_BACK = RIGHT+18;
static int TOP_BACK = TOP+18;
static int BOTTOM_BACK = BOTTOM+18;
static int TOP_LEFT_BACK = TOP_LEFT+18;
static int TOP_RIGHT_BACK = TOP_RIGHT+18;
static int BOTTOM_LEFT_BACK = BOTTOM_LEFT+18;
static int BOTTOM_RIGHT_BACK = BOTTOM_RIGHT+18;

const int AXIS_UP = 1;
const int AXIS_DOWN = -1;
const int AXIS_LEFT = 2;
const int AXIS_RIGHT = -2;
const int AXIS_FRONT = 3;
const int AXIS_BACK = -3;

const int UP_X = 1;
const int DOWN_X = -1;
const int LEFT_Y = 2;
const int RIGHT_Y = -2;
const int LEFT_Z = 3;
const int RIGHT_Z = -3;

const int X_AXIS = 1;
const int Y_AXIS = 2;
const int Z_AXIS = 3;
// const int X_AXIS_NEG = 3;
// const int Y_AXIS_NEG = 4;
// const int Z_AXIS_NEG = 5;

static int nrRotations = 0;
static int key_move = -1;

static GLint uniformAnim;
static array<array<GLfloat,6*36>,27> vtxArray;

static int xAxisArray[27];
static int yAxisArray[27];
static int zAxisArray[27];

static int positionArray[3][3][3];

static vector<int> cubePieceRotationsArray[27];

static void initAxisArray() {
    // for(int i = 0; i < 27; i++) {
    //     cubePieceRotationsArray[i].clear();
    // }

    for (int i = 0; i < 27; i++ ) {
        xAxisArray[i] = AXIS_RIGHT;
        yAxisArray[i] = AXIS_UP;
        zAxisArray[i] = AXIS_FRONT;
    }

}

static void printAxisArray() {
  for (int u = 0; u < 27; u++) {
    for (int i = 0; i < (int) cubePieceRotationsArray[u].size(); i++) {
        cout << u << ": " << cubePieceRotationsArray[u].at(i) << endl;
    }
    cout << endl;
  }
}

static void fillRotationsArray(int cubePiece, int rotation) {
    //cout << cubePiece <<": " << endl;
    cubePieceRotationsArray[cubePiece].push_back(rotation);
}


static int getOrientationFromAxis(int i, glm::vec3 axis, int direction) {
    GLfloat x = axis.x;
    GLfloat y = axis.y;
    GLfloat z = axis.z;

    if(x == 1.0f) {
        if(direction == UP_X && xAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == UP_X && xAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == DOWN_X && xAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == DOWN_X && xAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == LEFT_Z && xAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == LEFT_Z && xAxisArray[i] == AXIS_BACK) {
            return -1;
        } else if(direction == RIGHT_Z && xAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == RIGHT_Z && xAxisArray[i] == AXIS_BACK) {
            return -1;
        }

    } else if(y == 1.0f) {
        if(direction == UP_X && yAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == UP_X && yAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == DOWN_X && yAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == DOWN_X && yAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == LEFT_Z && yAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == LEFT_Z && yAxisArray[i] == AXIS_BACK) {
            return -1;
        } else if(direction == RIGHT_Z && yAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == RIGHT_Z && yAxisArray[i] == AXIS_BACK) {
            return -1;
        }

    } else if(z == 1.0f) {
        if(direction == UP_X && zAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == UP_X && zAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == DOWN_X && zAxisArray[i] == AXIS_LEFT) {
            return -1;
        } else if(direction == DOWN_X && zAxisArray[i] == AXIS_RIGHT) {
            return 1;
        } else if(direction == LEFT_Z && zAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == LEFT_Z && zAxisArray[i] == AXIS_BACK) {
            return -1;
        } else if(direction == RIGHT_Z && zAxisArray[i] == AXIS_FRONT) {
            return 1;
        } else if(direction == RIGHT_Z && zAxisArray[i] == AXIS_BACK) {
            return -1;
        }
    }
    return 1;
}

static glm::vec3 calcAxis(int i, glm::vec3 axis, int direction) {

    int x = xAxisArray[i];
    int y = yAxisArray[i];
    int z = zAxisArray[i];

    if(direction == LEFT_Z)  {
        if(x == AXIS_FRONT || x == AXIS_BACK) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_FRONT || y == AXIS_BACK) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_FRONT || z == AXIS_BACK) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    } else if(direction == RIGHT_Z) {
        if(x == AXIS_FRONT || x == AXIS_BACK) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_FRONT || y == AXIS_BACK) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_FRONT || z == AXIS_BACK) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    } else if(direction == UP_X) {
        if(x == AXIS_LEFT || x == AXIS_RIGHT) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_LEFT || y == AXIS_RIGHT) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_LEFT || z == AXIS_RIGHT) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    } else if(direction == DOWN_X) {
        if(x == AXIS_LEFT || x == AXIS_RIGHT) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_LEFT || y == AXIS_RIGHT) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_LEFT || z == AXIS_RIGHT) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    } else if(direction == LEFT_Y) {
        if(x == AXIS_UP || x == AXIS_DOWN) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_UP || y == AXIS_DOWN) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_UP || z == AXIS_DOWN) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    } else if(direction == RIGHT_Y) {
        if(x == AXIS_UP || x == AXIS_DOWN) {
           axis.x = 1.0f;
           axis.y = 0.0f;
           axis.z = 0.0f;
           return axis;
        } else if( y == AXIS_UP || y == AXIS_DOWN) {
           axis.x = 0.0f;
           axis.y = 1.0f;
           axis.z = 0.0f;
           return axis;
        } else if( z == AXIS_UP || z == AXIS_DOWN) {
           axis.x = 0.0f;
           axis.y = 0.0f;
           axis.z = 1.0f;
           return axis;
        }
    }

}

static void setAxis(int i, glm::vec3 axis, int direction) {

    GLfloat vX = axis.x;
    GLfloat vY = axis.y;
    GLfloat vZ = axis.z;

    int x = xAxisArray[i];
    int y = yAxisArray[i];
    int z = zAxisArray[i];
    // (vx, vy, vz)
    // _____________
    // ( 1, 0,  0) ->
    // ( 0, 1,  0)
    // ( 0, 0,  1)


    if(direction == LEFT_Z )  {
        if(vX == 1.0f) {
            if(x == AXIS_FRONT) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            } else if(x == AXIS_BACK) {
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_FRONT) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            } else if(y == AXIS_BACK) {
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            }
        } else if( vZ == 1.0f) {
            if(z == AXIS_FRONT) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            } else if(z == AXIS_BACK) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            }
        }
    } else if(direction == RIGHT_Z) {
        if(vX == 1.0f) {
            if(x == AXIS_FRONT) {
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            } else if(x == AXIS_BACK) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_FRONT) {
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            } else if(y == AXIS_BACK) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            }
        } else if( vZ == 1.0f) {
            if(z == AXIS_FRONT) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            } else if(z == AXIS_BACK) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            }
        }
    } else if(direction == UP_X) {
        if(vX == 1.0f) {
            if(x == AXIS_LEFT) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            } else if(x == AXIS_RIGHT) {
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_LEFT) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            } else if(y == AXIS_RIGHT) { // not validated
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            }
        } else if( vZ == 1.0f) { 
            if(z == AXIS_LEFT) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            } else if(z == AXIS_RIGHT) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            }
        }
    } else if(direction == DOWN_X) { // not validated
        if(vX == 1.0f) {
            if(x == AXIS_LEFT) {
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            } else if(x == AXIS_RIGHT) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_LEFT) {
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            } else if(y == AXIS_RIGHT) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            }
        } else if( vZ == 1.0f) { 
            if(z == AXIS_LEFT) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            } else if(z == AXIS_RIGHT) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            }
        }
    } else if(direction == LEFT_Y) {
        if(vX == 1.0f) {
            if(x == AXIS_UP) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            } else if(x == AXIS_DOWN) { // not validated
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_UP) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            } else if(y == AXIS_DOWN) {
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            }
        } else if( vZ == 1.0f) { 
            if(z == AXIS_UP) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            } else if(z == AXIS_DOWN) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            }
        }
    } else if(direction == RIGHT_Y) { // not validated
        if(vX == 1.0f) {
            if(x == AXIS_UP) {
                yAxisArray[i] = -z;
                zAxisArray[i] = y;
            } else if(x == AXIS_DOWN) {
                yAxisArray[i] = z;
                zAxisArray[i] = -y;
            }
        } else if( vY == 1.0f) {
            if(y == AXIS_UP) {
                xAxisArray[i] = z;
                zAxisArray[i] = -x;
            } else if(y == AXIS_DOWN) {
                xAxisArray[i] = -z;
                zAxisArray[i] = x;
            }
        } else if( vZ == 1.0f) { 
            if(z == AXIS_UP) {
                xAxisArray[i] = -y;
                yAxisArray[i] = x;
            } else if(z == AXIS_DOWN) {
                xAxisArray[i] = y;
                yAxisArray[i] = -x;
            }
        }
    }


}

static void initPositionArray() {
    unsigned x, y, z, depth;

    for (y = 0; y < 3; y++) {
        depth = 9*y;
        for (z = 0; z < 3; z++) {
            for (x = 0; x < 3; x++) {
                if (z == 0) {
                    if (x == 0) {
                        positionArray[x][y][z] = BOTTOM_LEFT + depth;
                    }
                    else if (x == 1) {
                        positionArray[x][y][z] = BOTTOM + depth;
                    }
                    else if (x == 2) {
                        positionArray[x][y][z] = BOTTOM_RIGHT + depth;
                    }
                }
                else if (z == 1) {
                    if (x == 0) {
                        positionArray[x][y][z] = LEFT + depth;
                    }
                    else if (x == 1) {
                        positionArray[x][y][z] = MIDDLE + depth;
                    }
                    else if (x == 2) {
                        positionArray[x][y][z] = RIGHT + depth;
                    }
                }
                else if (z == 2) {
                    if (x == 0) {
                        positionArray[x][y][z] = TOP_LEFT + depth;
                    }
                    else if (x == 1) {
                        positionArray[x][y][z] = TOP + depth;
                    }
                    else if (x == 2) {
                        positionArray[x][y][z] = TOP_RIGHT + depth;
                    }
                }
            }
        }
    }
}
/*                                                                           */
/* GLFW callback functions for event handling                                */
/*                                                                           */
static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


static void keyCallback(GLFWwindow* myWindow, int key, int scanCode,
                        int action, int mod)
{
    if (((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q))  &&
        (action == GLFW_PRESS)){
    /* close window upon hitting the escape key or Q/q */
        glfwSetWindowShouldClose(myWindow, GL_TRUE);

    } else if ((key == GLFW_KEY_A) && action == GLFW_PRESS) {
        key_move = 6;
    } else if ((key == GLFW_KEY_S) && action == GLFW_PRESS) {
        key_move = 7;
    } else if ((key == GLFW_KEY_D) && action == GLFW_PRESS) {
        key_move = 8;
    } else if ((key == GLFW_KEY_F) && action == GLFW_PRESS) {
        key_move = 9;
    } else if ((key == GLFW_KEY_G) && action == GLFW_PRESS) {
        key_move = 10;
    } else if ((key == GLFW_KEY_H) && action == GLFW_PRESS) {
        key_move = 11;
    }
}

static void cursorPosCallBack (GLFWwindow* myWindow, double x_pos, double y_pos)
{
    printf("Mouse is at (%6.1f, %6.1f) \n", x_pos, y_pos);
}

static void mouseButtonCallBack(GLFWwindow* myWindow, int button, int action, int mods) {
    if((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
        double x_pos, y_pos;
        glfwGetCursorPos(myWindow, &x_pos, &y_pos);
        printf("Left mouse button pressed at (%6.1f, %6.1f) \n", x_pos, y_pos);
    }
}


bool checkShaderCompileStatus(GLuint shaderID)
{
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        GLchar* log = new char[length + 1];
        glGetShaderInfoLog(shaderID, length, &length, &log[0]);
        fprintf(stderr, "%s", log);
        return false;
    }
    return true;
}


bool checkShaderProgramLinkStatus(GLuint programID)
{
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        GLchar* log = new char[length + 1];
        glGetProgramInfoLog(programID, length, &length, &log[0]);
        fprintf(stderr, "%s", log);
        return false;
    }
    return true;
}

glm::vec3* getDirectionRightUp(GLFWwindow* myWindow, glm::vec3 position, GLfloat horizontalAngle, GLfloat verticalAngle, GLfloat initialFoV, GLfloat speed, GLfloat mouseSpeed, GLfloat deltaTime)
{
    static glm::vec3 look[3];
    double x_pos, y_pos;

    glfwGetCursorPos(myWindow, &x_pos, &y_pos);
    //glfwSetCursorPos(myWindow, 800/2, 600/2);

    horizontalAngle += mouseSpeed * deltaTime * float(800/2 - x_pos);
    verticalAngle   += mouseSpeed * deltaTime * float(600/2 - y_pos);

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(myWindow, GLFW_KEY_UP) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(myWindow, GLFW_KEY_DOWN) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(myWindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(myWindow, GLFW_KEY_LEFT) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

    //float FoV = initialFoV - 5 * glfwGetMouseWheel(myWindow);
    look[0] = direction;
    look[1] = right;
    look[2] = up;

    return look;
}

void changeCubePositions(int move) {
  // int tmp_top = TOP;
  // int tmp_topleft = TOP_LEFT;
  // int tmp_topright = TOP_RIGHT;
  //
  // if (direction == 1 && row == 0) {
  //
  //   TOP_RIGHT = BOTTOM_RIGHT;
  //   std::cout << "TOPRIGHT: " << TOP_RIGHT << std::endl;
  //   TOP_LEFT = tmp_topright;
  //   std::cout << "TOPLEFT: " << TOP_LEFT << std::endl;
  //   TOP = RIGHT;
  //   std::cout << "TOP: " << TOP << std::endl;
  //   BOTTOM_RIGHT = BOTTOM_LEFT;
  //   std::cout << "BOTTOMRIGHT: " << BOTTOM_RIGHT << std::endl;
  //   BOTTOM_LEFT = tmp_topleft;
  //   std::cout << "BOTTOMLEFT: " << BOTTOM_LEFT << std::endl;
  //   RIGHT = BOTTOM;
  //   std::cout << "RIGHT: " << RIGHT << std::endl;
  //   BOTTOM = LEFT;
  //   std::cout << "BOTTOM: " << BOTTOM << std::endl;
  //   LEFT = tmp_top;
  //   std::cout << "LEFT: " << LEFT << std::endl;
  // }
  unsigned x, y, z;
  int tmp;

  if (move == 6) {
      x = 0;
      // moving corner pieces
      tmp = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][2][1];
      positionArray[x][2][1] = positionArray[x][1][2];
      positionArray[x][1][2] = tmp;
  }
  else if (move == 7) {
      x = 1;
      // moving corner pieces
      tmp = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][2][1];
      positionArray[x][2][1] = positionArray[x][1][2];
      positionArray[x][1][2] = tmp;
  }
  else if (move == 8) {
      x = 2;
      // moving corner pieces
      tmp = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][2][1];
      positionArray[x][2][1] = positionArray[x][1][2];
      positionArray[x][1][2] = tmp;
  }
  else if (move == 9) {
      x = 0;
      // moving corner pieces
      tmp = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][2];
      positionArray[x][1][2] = positionArray[x][2][1];
      positionArray[x][2][1] = tmp;
  }
  else if (move == 10) {
      x = 1;
      // moving corner pieces
      tmp = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][2];
      positionArray[x][1][2] = positionArray[x][2][1];
      positionArray[x][2][1] = tmp;
  }
  else if (move == 11) {
      x = 2;
      // moving corner pieces
      tmp = positionArray[x][2][0];
      positionArray[x][2][0] = positionArray[x][0][0];
      positionArray[x][0][0] = positionArray[x][0][2];
      positionArray[x][0][2] = positionArray[x][2][2];
      positionArray[x][2][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[x][1][0];
      positionArray[x][1][0] = positionArray[x][0][1];
      positionArray[x][0][1] = positionArray[x][1][2];
      positionArray[x][1][2] = positionArray[x][2][1];
      positionArray[x][2][1] = tmp;
  }
  else if (move == 12) {
      y = 0;
      // moving corner pieces
      tmp = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[2][y][1];
      positionArray[2][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = tmp;
  }
  else if (move == 13) {
      y = 1;
      // moving corner pieces
      tmp = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[2][y][1];
      positionArray[2][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = tmp;
  }
  else if (move == 14) {
      y = 2;
      // moving corner pieces
      tmp = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[2][y][1];
      positionArray[2][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = tmp;
  }
  else if (move == 15) {
      y = 0;
      // moving corner pieces
      tmp = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = positionArray[2][y][1];
      positionArray[2][y][1] = tmp;
  }
  else if (move == 16) {
      y = 1;
      // moving corner pieces
      tmp = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = positionArray[2][y][1];
      positionArray[2][y][1] = tmp;
  }
  else if (move == 17) {
      y = 2;
      // moving corner pieces
      tmp = positionArray[2][y][0];
      positionArray[2][y][0] = positionArray[0][y][0];
      positionArray[0][y][0] = positionArray[0][y][2];
      positionArray[0][y][2] = positionArray[2][y][2];
      positionArray[2][y][2] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][y][0];
      positionArray[1][y][0] = positionArray[0][y][1];
      positionArray[0][y][1] = positionArray[1][y][2];
      positionArray[1][y][2] = positionArray[2][y][1];
      positionArray[2][y][1] = tmp;
  }
  else if (move == 18) {
      z = 0;
      // moving corner pieces
      tmp = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[2][1][z];
      positionArray[2][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = tmp;
  }
  else if (move == 19) {
      z = 1;
      // moving corner pieces
      tmp = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[2][1][z];
      positionArray[2][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = tmp;
  }
  else if (move == 20) {
      z = 2;
      // moving corner pieces
      tmp = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[2][1][z];
      positionArray[2][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = tmp;
  }
  else if (move == 21) {
      z = 0;
      // moving corner pieces
      tmp = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = positionArray[2][1][z];
      positionArray[2][1][z] = tmp;
  }
  else if (move == 22) {
      z = 1;
      // moving corner pieces
      tmp = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = positionArray[2][1][z];
      positionArray[2][1][z] = tmp;
  }
  else if (move == 23) {
      z = 2;
      // moving corner pieces
      tmp = positionArray[2][0][z];
      positionArray[2][0][z] = positionArray[0][0][z];
      positionArray[0][0][z] = positionArray[0][2][z];
      positionArray[0][2][z] = positionArray[2][2][z];
      positionArray[2][2][z] = tmp;

      // moving edge positionArray
      tmp = positionArray[1][0][z];
      positionArray[1][0][z] = positionArray[0][1][z];
      positionArray[0][1][z] = positionArray[1][2][z];
      positionArray[1][2][z] = positionArray[2][1][z];
      positionArray[2][1][z] = tmp;
  }

}

void createAnim(GLuint shaderProgram, glm::mat4 anim) {
  const char* uniformName = "anim";
  uniformAnim = glGetUniformLocation(shaderProgram, uniformName);
  if (uniformAnim == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
}

glm::mat4 rotLeftY(glm::mat4 anim, float orientation, glm::vec3 rot) {
    float angle = 1.0f * orientation;

    anim = glm::rotate(anim, glm::radians(angle), rot);

    return anim;
}

glm::mat4 rotUpX(glm::mat4 anim, float orientation, glm::vec3 axis) {
    float angle = 1.0f * orientation;

    anim = glm::translate(anim, glm::vec3(0.0f, 0.0f, -2.1f) );
    anim = glm::rotate(anim, glm::radians(angle), axis);
    anim = glm::translate(anim, -(glm::vec3(0.0f, 0.0f, -2.1f)) );


    return anim;
}

glm::mat4 rotLeftZ(glm::mat4 anim, float orientation, glm::vec3 axis) {
    float angle = 1.0f * orientation;

    anim = glm::translate(anim, glm::vec3(-2.1f, 0.0f, -4.2f));
    anim = glm::translate(anim, glm::vec3(2.1f, 0.0f, 2.1f));
    anim = glm::rotate(anim, glm::radians(angle), axis);
    anim = glm::translate(anim, -(glm::vec3(2.1f, 0.0f, 2.1f)));
    anim = glm::translate(anim, -(glm::vec3(-2.1f, 0.0f, -4.2f)));


    return anim;
}

glm::mat4 spinX2(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    //glm::vec3 new_rot;
    int sign = 1;
    //right Row
    if(i == positionArray[2][0][0] || i == positionArray[2][0][1] || i == positionArray[2][0][2]
        || i == positionArray[2][1][0] || i == positionArray[2][1][1] || i == positionArray[2][1][2]
        || i == positionArray[2][2][0] || i == positionArray[2][2][1] || i == positionArray[2][2][2]) {

        rot = glm::vec3(1.0f, 0.0f, 0.0f);

        if(orientation == -1.0) {

            rot = calcAxis(i, rot, UP_X);

            sign = getOrientationFromAxis(i, rot, UP_X);

            // if(i == BOTTOM_LEFT) {
            //     cout << i << ": " << rot.x << rot.y << rot.z << endl;
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            // }

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, UP_X);

                if(i == BOTTOM)
                    cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        } else {
            rot = calcAxis(i, rot, DOWN_X);

            sign = getOrientationFromAxis(i, rot, DOWN_X);

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, DOWN_X);

                // if(i == BOTTOM)
                //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }
        anim = rotUpX(anim, orientation*sign, rot);
            // cout << nrRotations << endl;
        nrRotations +=1;
        // }

        // glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));

    }

    return anim;
}

glm::mat4 spinX1(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    int sign;
    
    if(i == positionArray[1][0][0] || i == positionArray[1][0][1] || i == positionArray[1][0][2]
        || i == positionArray[1][1][0] || i == positionArray[1][1][1] || i == positionArray[1][1][2]
        || i == positionArray[1][2][0] || i == positionArray[1][2][1] || i == positionArray[1][2][2]) {

        rot = glm::vec3(1.0f, 0.0f, 0.0f);
        if(orientation == -1.0) {

            rot = calcAxis(i, rot, UP_X);

            sign = getOrientationFromAxis(i, rot, UP_X);

            // if(i == BOTTOM_LEFT) {
            //     cout << i << ": " << rot.x << rot.y << rot.z << endl;
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            // }

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, UP_X);

                if(i == BOTTOM)
                    cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        } else {
            rot = calcAxis(i, rot, DOWN_X);

            sign = getOrientationFromAxis(i, rot, DOWN_X);

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, DOWN_X);

                // if(i == BOTTOM)
                //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }
        // createAnim(shaderProgram, anim);
        // if(nrRotations <= 90*9) {
        // if(i == RIGHT || i == TOP_RIGHT || i == BOTTOM_RIGHT) {
        //     rot = glm::vec3(0.0f, 1.0f, 0.0f);
        //     orientation *= -1;
        // } else {
        anim = rotUpX(anim, orientation*sign, rot);
            // cout << nrRotations << endl;
        nrRotations +=1;
        // }

        // glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));

    }

    return anim;
}

glm::mat4 spinX0(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    int sign;

    //right Row
    if(i == positionArray[0][0][0] || i == positionArray[0][0][1] || i == positionArray[0][0][2]
        || i == positionArray[0][1][0] || i == positionArray[0][1][1] || i == positionArray[0][1][2]
        || i == positionArray[0][2][0] || i == positionArray[0][2][1] || i == positionArray[0][2][2]) {

        rot = glm::vec3(1.0f, 0.0f, 0.0f);
        if(orientation == -1.0) {

            rot = calcAxis(i, rot, UP_X);

            sign = getOrientationFromAxis(i, rot, UP_X);

            // if(i == BOTTOM_LEFT) {
            //     cout << i << ": " << rot.x << rot.y << rot.z << endl;
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            // }

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, UP_X);

                // if(i == BOTTOM)
                //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        } else {
            rot = calcAxis(i, rot, DOWN_X);

            sign = getOrientationFromAxis(i, rot, DOWN_X);

            if(nrRotations >= 90*9-9) {
                setAxis(i, rot, DOWN_X);

                // if(i == BOTTOM)
                //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }
        // createAnim(shaderProgram, anim);
        // if(nrRotations <= 90*9) {
        // if(i == RIGHT || i == TOP_RIGHT || i == BOTTOM_RIGHT) {
        //     rot = glm::vec3(0.0f, 1.0f, 0.0f);
        //     orientation *= -1;
        // } else {
        cout << sign << endl;
        anim = rotUpX(anim, orientation*sign, rot);
            // cout << nrRotations << endl;
        nrRotations +=1;
        // }

        // glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));

    }

    return anim;
}

glm::mat4 spinZ0(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    int sign;

    if(i == positionArray[0][0][0] || i == positionArray[1][0][0] || i == positionArray[2][0][0]
        || i == positionArray[0][0][1] || i == positionArray[1][0][1] || i == positionArray[2][0][1]
        || i == positionArray[0][0][2] || i == positionArray[1][0][2] || i == positionArray[2][0][2]) {
        // if(nrRotations <= 90*9) {

        // if (numCalculated < 9){
        //     fillRotationsArray(i, LEFT_Y);
        //     numCalculated++;
        // }

        rot = glm::vec3(0.0f, 0.0f, 1.0f); //TODO: initial


        if(orientation == 1.0) {

            rot = calcAxis(i, rot, LEFT_Z);
            
            sign = getOrientationFromAxis(i, rot, LEFT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Z);
            }

        } else {
            rot = calcAxis(i, rot, RIGHT_Z);

            sign = getOrientationFromAxis(i, rot, RIGHT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i,rot, RIGHT_Z);

            }
        }

        // cout << sign << endl;
        anim = rotLeftY(anim, orientation*sign, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

glm::mat4 spinZ1(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    int sign; 

    if(i == positionArray[0][1][0] || i == positionArray[1][1][0] || i == positionArray[2][1][0]
        || i == positionArray[0][1][1] || i == positionArray[1][1][1] || i == positionArray[2][1][1]
        || i == positionArray[0][1][2] || i == positionArray[1][1][2] || i == positionArray[2][1][2]) {
        // if(nrRotations <= 90*9) {


        rot = glm::vec3(0.0f, 0.0f, 1.0f);

        if(orientation == 1.0) {

            rot = calcAxis(i, rot, LEFT_Z);
            
            sign = getOrientationFromAxis(i, rot, LEFT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Z);
            }

        } else {
            rot = calcAxis(i, rot, RIGHT_Z);

            sign = getOrientationFromAxis(i, rot, RIGHT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i,rot, RIGHT_Z);

            }
        }
        anim = rotLeftY(anim, orientation*sign, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

glm::mat4 spinZ2(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    int sign;

    if(i == positionArray[0][2][0] || i == positionArray[1][2][0] || i == positionArray[2][2][0]
        || i == positionArray[0][2][1] || i == positionArray[1][2][1] || i == positionArray[2][2][1]
        || i == positionArray[0][2][2] || i == positionArray[1][2][2] || i == positionArray[2][2][2]) {
        // if(nrRotations <= 90*9) {



        rot = glm::vec3(0.0f, 0.0f, 1.0f);
       if(orientation == 1.0) {

            rot = calcAxis(i, rot, LEFT_Z);
            
            sign = getOrientationFromAxis(i, rot, LEFT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Z);
            }

        } else {
            rot = calcAxis(i, rot, RIGHT_Z);

            sign = getOrientationFromAxis(i, rot, RIGHT_Z);

            if(nrRotations >= 90*9-9) {
               setAxis(i,rot, RIGHT_Z);

            }
        }

        anim = rotLeftY(anim, orientation*sign, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

glm::mat4 spinY2(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    if(i == positionArray[0][0][2] || i == positionArray[1][0][2] || i == positionArray[2][0][2]
        || i == positionArray[0][1][2] || i == positionArray[1][1][2] || i == positionArray[2][1][2]
        || i == positionArray[0][2][2] || i == positionArray[1][2][2] || i == positionArray[2][2][2]) {
        // if(nrRotations <= 90*9) {

        rot = glm::vec3(0.0f, 1.0f, 0.0f);

        if (orientation == 1.0) { // counter clockwise
            rot = calcAxis(i, rot, LEFT_Y);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Y);
            //    if(i == BOTTOM_LEFT)
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }

        anim = rotLeftZ(anim, orientation, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

glm::mat4 spinY1(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    if(i == positionArray[0][0][1] || i == positionArray[1][0][1] || i == positionArray[2][0][1]
        || i == positionArray[0][1][1] || i == positionArray[1][1][1] || i == positionArray[2][1][1]
        || i == positionArray[0][2][1] || i == positionArray[1][2][1] || i == positionArray[2][2][1]) {
        // if(nrRotations <= 90*9) {

        rot = glm::vec3(0.0f, 1.0f, 0.0f);
        if (orientation == 1.0) { // counter clockwise
            rot = calcAxis(i, rot, LEFT_Y);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Y);
            //    if(i == BOTTOM_LEFT)
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }
        anim = rotLeftZ(anim, orientation, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

glm::mat4 spinY0(glm::mat4 anim, float orientation, int i) {
    glm::vec3 rot;
    glm::vec3 new_rot;
    if(i == positionArray[0][0][0] || i == positionArray[1][0][0] || i == positionArray[2][0][0]
        || i == positionArray[0][1][0] || i == positionArray[1][1][0] || i == positionArray[2][1][0]
        || i == positionArray[0][2][0] || i == positionArray[1][2][0] || i == positionArray[2][2][0]) {
        // if(nrRotations <= 90*9) {

        rot = glm::vec3(0.0f, 1.0f, 0.0f);
        if (orientation == 1.0) { // counter clockwise
            rot = calcAxis(i, rot, LEFT_Y);

            if(nrRotations >= 90*9-9) {
               setAxis(i, rot, LEFT_Y);
            //    if(i == BOTTOM_LEFT)
            //     cout << "X: " << xAxisArray[i] << " Y: " << yAxisArray[i] << " Z: " << zAxisArray[i] << endl;
            }
        }
        anim = rotLeftZ(anim, orientation, rot);
        // cout << nrRotations << endl;
        nrRotations +=1;
        //    glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        // }
    }
    // }

    return anim;
}

// glm::mat4 handleCube(GLfloat* vtx[], GLuint VAOArray[], GLuint VBOArray[], glm::mat4 anim, int arraySize, bool state) {
//     for(int i = 0; i < arraySize; i+=1) {

//         if(i == 1) {
//         //    anim = spinObj(anim, state);
//         }

//         glBufferData(GL_ARRAY_BUFFER, 6*36*4, vtx[i], GL_STATIC_DRAW);
//         glDrawArrays(GL_TRIANGLES, 0, 36);
//     }
//     return anim;
// }



void printCube(GLfloat* vtx) {
  for(int i = 0; i+5 < 6*36; i+=6) {
    printf("%f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2]);
  }
}

void printCube(std::array<GLfloat,6*36> vtx) {
  for(int i = 0; i+5 < 6*36; i+=6) {
    printf("%f, %f, %f \n", vtx[i], vtx[i+1], vtx[i+2]);
  }
}


int main()
{
    /* window dimensions */
    const GLuint WIDTH = 800, HEIGHT = 600;

    /*                                                                        */
    /* initialization and set-up                                              */
    /*                                                                        */
    /* initialization of GLFW */
    glfwSetErrorCallback(errorCallback);
    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Cannot initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    /* set some GLFW options: we require OpenGL 3.3 (or more recent) context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* create GFLW window (monitor in windowed mode), do not share resources */
    GLFWwindow* myWindow = glfwCreateWindow(WIDTH, HEIGHT, "Rubikscube",
                                            NULL, NULL);
    if (myWindow == NULL) {
        fprintf(stderr, "Cannot open GLFW window\n");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(myWindow);

    /* initialization of GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glewStatus));
        exit(EXIT_FAILURE);
    }

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: GPU does not support GLEW 2.0\n");
        exit(EXIT_FAILURE);
    }

    // int vtxSize = 6*36;
    int arraySize = 27;
    static Cube cube[] ={
        Cube(MIDDLE, 0.0f),
        Cube(LEFT, 0.0f),
        Cube(RIGHT, 0.0f),
        Cube(TOP, 0.0f),
        Cube(BOTTOM, 0.0f),
        Cube(TOP_LEFT, 0.0f),
        Cube(TOP_RIGHT, 0.0f),
        Cube(BOTTOM_LEFT, 0.0f),
        Cube(BOTTOM_RIGHT, 0.0f),
        Cube(MIDDLE, -2.2f),
        Cube(LEFT, -2.2f),
        Cube(RIGHT, -2.2f),
        Cube(TOP, -2.2f),
        Cube(BOTTOM, -2.2f),
        Cube(TOP_LEFT, -2.2f),
        Cube(TOP_RIGHT, -2.2f),
        Cube(BOTTOM_LEFT, -2.2f),
        Cube(BOTTOM_RIGHT, -2.2f),
        Cube(MIDDLE, -4.4f),
        Cube(LEFT, -4.4f),
        Cube(RIGHT, -4.4f),
        Cube(TOP, -4.4f),
        Cube(BOTTOM, -4.4f),
        Cube(TOP_LEFT, -4.4f),
        Cube(TOP_RIGHT, -4.4f),
        Cube(BOTTOM_LEFT, -4.4f),
        Cube(BOTTOM_RIGHT, -4.4f),
        // Cube(BOTTOM, 0.0f)
    };

    //std::vector<GLfloat> vtxArray;


    GLuint myVAO[arraySize];
    glGenVertexArrays(arraySize, &myVAO[0]);
    // glBindVertexArray(myVAO[0]);

    /* generate and bind one Vertex Buffer Object */
    GLuint myVBO[arraySize];
    glGenBuffers(arraySize, &myVBO[0]);

    /* copy the vertex data to it */

    for(int i = 0; i < arraySize; i++) {

       vtxArray[i] = cube[i].createCubes();

        glBindVertexArray(myVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, myVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, vtxArray[i].size()*4, &vtxArray[i], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    }

    /* OpenGL settings */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* define and compile the vertex shader */
    const char* vertexShaderSource = GLSL(
      layout(location=0) in vec3 position;
      //in vec2 textureCoordIn;
      layout(location=1) in vec3 colorVtxIn;
      uniform mat4 proj;
      uniform mat4 view;
      uniform mat4 anim;
      out vec3 colorVtxOut;
      //out vec2 textureCoordOut;
      void main() {
        //textureCoordOut = vec2(textureCoordIn.x,
        //                     1.0 - textureCoordIn.y);
        colorVtxOut = colorVtxIn;
        gl_Position = proj * view * anim * vec4(position, 1.0);
    }
                                          );
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /* check whether the vertex shader compiled without an error */
    if (!checkShaderCompileStatus(vertexShader)) {
        fprintf(stderr, "Vertex shader did not compile\n");
        exit(EXIT_FAILURE);
    }

    /* define and compile the fragment shader */
    const char* fragmentShaderSource = GLSL(
        in vec3 colorVtxOut;
        out vec4 outColor;
        void main() {
            outColor = vec4(colorVtxOut, 1.0f);
    }
                                            );
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* check whether the fragment shader compiled without an error */
    if (!checkShaderCompileStatus(fragmentShader)) {
        fprintf(stderr, "Fragment shader did not compile\n");
        exit(EXIT_FAILURE);
    }

    /* create a shader program by linking the vertex and fragment shader */
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    /* check whether the shader program linked without an error */
    if (!checkShaderProgramLinkStatus(shaderProgram)) {
        fprintf(stderr, "Shader did not link\n");
        exit(EXIT_FAILURE);
    }

    /* make the shader program active */
    glUseProgram(shaderProgram);

    /* define how the input is organized */
    const char* attributeName;
    attributeName = "position";
    GLint posAttrib = glGetAttribLocation(shaderProgram, attributeName);
    if (posAttrib == -1) {
        fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
        exit(EXIT_FAILURE);
    }
    // glEnableVertexAttribArray(posAttrib);
    // glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
    //                       6 * sizeof(GLfloat), 0);

    attributeName = "colorVtxIn";
    GLint colAttrib = glGetAttribLocation(shaderProgram, attributeName);
    if (colAttrib == -1) {
        fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
    }
    // glEnableVertexAttribArray(colAttrib);
    // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
    //                       6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

    /*attributeName = "textureCoordIn";
     GLint texAttrib = glGetAttribLocation(shaderProgram, attributeName);
     if (texAttrib == -1) {
     fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
     exit(EXIT_FAILURE);
     }
     glEnableVertexAttribArray(texAttrib);
     glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
     5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));*/

    /* load texture image */
    /*GLint texWidth, texHeight;
     GLint channels;*/
    /*unsigned char* texImage = SOIL_load_image("../img/katze.png",
     &texWidth, &texHeight, &channels,
     SOIL_LOAD_RGB);
     if (texImage == NULL) {
     fprintf(stderr, "Image file could no_BUFFER, vtxSize2*4, vtx2, GL_STATIC_DRAW);

        // glDrawArrays(GL_TRIANGLES, 0, 12*36);

     }*/

    /* generate texture */
    /*GLuint textureID;
     glActiveTexture(GL_TEXTURE0);
     glGenTextures(1, &textureID);
     glBindTexture(GL_TEXTURE_2D, textureID);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,
     GL_UNSIGNED_BYTE, texImage);
     SOIL_free_image_data(texImage);*/

    /* set texture parameters */
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

    /* define a view transformation */

    // position
    glm::vec3 position = glm::vec3( 0, 0, 5 );
    // horizontal angle : toward -Z
    GLfloat horizontalAngle = 3.14f;
    // vertical angle : 0, look at the horizon
    GLfloat verticalAngle = 0.0f;
    // Initial Field of View
    GLfloat initialFoV = 45.0f;

    GLfloat speed = 3.0f; // 3 units / second
    GLfloat mouseSpeed = 0.005f;
    GLfloat lastTime = 0;
    glm::vec3* look;
    GLfloat deltaTime = 0;


    glm::mat4 view = glm::lookAt(glm::vec3(4.0f, 4.0f, 6.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    //glm::mat4 view = glm::lookAt(look[0], look[1], look[2]);

    /* define a  projection transformation */
    glm::mat4 proj = glm::perspective(glm::radians(100.0f), 4.0f/3.0f, 0.1f, 40.0f);

    /* define a transformation matrix for the animation */
    glm::mat4 anim = glm::mat4(1.0f);

    // glm::mat4 anim2 = glm::mat4(1.0f);

    // glm::mat4 anim3 = glm::mat4(1.0f);

    // glm::mat4 anim4 = glm::mat4(1.0f);

    /* bind uniforms and pass data to the shader program */
    const char* uniformName;
    /*uniformName = "textureData";
     GLint uniformTex = glGetUniformLocation(shaderProgram, uniformName);
     if (uniformTex == -1) {
     fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
     exit(EXIT_FAILURE);
     }
     glUniform1i(uniformTex, 0);*/

    uniformName = "view";
    GLint uniformView = glGetUniformLocation(shaderProgram, uniformName);
    if (uniformView == -1) {
        fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
        exit(EXIT_FAILURE);
    }
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

    uniformName = "proj";
    GLint uniformProj = glGetUniformLocation(shaderProgram, "proj");
    if (uniformProj == -1) {
        fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
        exit(EXIT_FAILURE);
    }
    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));

    /* register callback functions */
    glfwSetKeyCallback(myWindow, keyCallback);
    //glfwSetCursorPosCallback(myWindow, cursorPosCallBack);
    glfwSetMouseButtonCallback(myWindow, mouseButtonCallBack);

    /*                                                                        */
    /* event-handling and rendering loop                                      */
    /*                                                                        */

    // bool state = true;
    glm::mat4 myAnim;
    nrRotations = 0;

    std::vector<int> moves { 15, 8, 22, 7, 6, 10, 9, 0};
    // std::vector<int> moves {6, 15, 12, 0};
    int move = 0;
    array<glm::mat4,27> animArray;

    for(int i = 0; i < (int) animArray.size(); i++) {
        animArray[i] = anim;
    }

    int vecCounter = 0;
    initPositionArray();
    initAxisArray();
    // createAnim(shaderProgram, anim2);
    while (!glfwWindowShouldClose(myWindow)) {
        if (vecCounter < (int) moves.size()) {
            move = moves.at(vecCounter);
        }

        // if (key_move != -1) {
        //     move = key_move;
        // }

        /* set the window background to black */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < arraySize; i+=1) {   //TODO: Cube Array aufteilen mit veränderte und unveränderte Cubes IDEE!
            glBindVertexArray(myVAO[i]);
            //glBindBuffer(GL_ARRAY_BUFFER, myVBO[i]);
            myAnim = animArray[i];
            // cout << glm::to_string(myAnim) << endl;
            // createAnim(shaderProgram, anim);

            if(move == 6) {
                myAnim = spinX0(myAnim, -1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 7) {
                myAnim = spinX1(myAnim, -1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 8) {
                myAnim = spinX2(myAnim, -1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            }else if(move == 9) {
                myAnim = spinX0(myAnim, 1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 10) {
                myAnim = spinX1(myAnim, 1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 11) {
                myAnim = spinX2(myAnim, 1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 12) {
                myAnim = spinZ0(myAnim, -1.0, i);
                animArray[i] = myAnim;
            } else if(move == 13) {
                myAnim = spinZ1(myAnim, -1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 14) {
                myAnim = spinZ2(myAnim, -1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 15) {
                myAnim = spinZ0(myAnim, 1.0, i);
                animArray[i] = myAnim;
            } else if(move == 16) {
                myAnim = spinZ1(myAnim, 1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 17) {
                myAnim = spinZ2(myAnim, 1.0, i);
                // nrRotations = 0;
                animArray[i] = myAnim;
            } else if(move == 18) {
                myAnim = spinY0(myAnim, -1.0, i);
                animArray[i] = myAnim;
            } else if(move == 19) {
                myAnim = spinY1(myAnim, -1.0, i);
                animArray[i] = myAnim;
            } else if(move == 20) {
                myAnim = spinY2(myAnim, -1.0, i);
                animArray[i] = myAnim;
            } else if(move == 21) {
                myAnim = spinY0(myAnim, 1.0, i);
                animArray[i] = myAnim;
            } else if(move == 22) {
                myAnim = spinY1(myAnim, 1.0, i);
                animArray[i] = myAnim;
            } else if(move == 23) {
                myAnim = spinY2(myAnim, 1.0, i);
                animArray[i] = myAnim;
            }
            // printf("/-----------------------------------------/ \n");

            // cout << glm::to_string(myAnim) << endl;
            // printf("/-----------------------------------------/ \n");


            glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(myAnim));
            glDrawArrays(GL_TRIANGLES, 0, 6*36);
        }
        // printCube(vtxArray[1]);
        // printf("/-----------------------------------------/ \n");
        // printCube(vtxArray[2]);
        // printf("/-----------------------------------------/ \n");
        // break;
        // move ++;
        if(nrRotations == 90*9) {
            changeCubePositions(move);
            nrRotations = 0;
            vecCounter += 1;
            numCalculated = 0;
            // printAxisArray();
            key_move = -1;
        }


        deltaTime = GLfloat(glfwGetTime() - lastTime);
        lastTime = deltaTime;

        look = getDirectionRightUp(myWindow, position, horizontalAngle, verticalAngle, initialFoV, speed, mouseSpeed, deltaTime);
        view = glm::lookAt(look[0], look[1], look[2]);

        /* Swap buffers */
        glfwSwapBuffers(myWindow);

        /* poll events */
        glfwPollEvents();

        // cout << "/--------------------------------------------------/" << endl;
    }

    /*                                                                        */
    /* clean-up and release resources                                         */
    /*                                                                        */
    //glDeleteTextures(1, &textureID);

    glUseProgram(0);
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteProgram(shaderProgram);

    for(int i = 0; i < 5; i+=1) {
        glDeleteBuffers(arraySize,myVBO);
        // glDeleteBuffers(1, &myVBO);

        glDeleteVertexArrays(arraySize, myVAO);
    }



    /*                                                                        */
    /* termination of GLFW                                                    */
    /*                                                                        */
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
