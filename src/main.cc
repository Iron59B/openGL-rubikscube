/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <vector>
#include <iostream>

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

#define GLSL(src) "#version 330 core\n" #src
#define GLM_FORCE_RADIANS

const int MIDDLE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int TOP = 3;
const int BOTTOM = 4;
const int TOP_LEFT = 5;
const int TOP_RIGHT = 6;
const int BOTTOM_LEFT = 7;
const int BOTTOM_RIGHT = 8;

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
        (action == GLFW_PRESS))
    /* close window upon hitting the escape key or Q/q */
        glfwSetWindowShouldClose(myWindow, GL_TRUE);
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

glm::mat4 spinObj(glm::mat4 anim, bool state) {
    float angle = 1.0f;

    if(state == true) {
    // anim = glm::translate(anim, glm::vec3(0.0f, 0.0f, 1.0f) );
        anim = glm::rotate(anim, glm::radians(angle), glm::vec3(0.0f, 0.0f, 2.0f));
    // anim = glm::translate(anim, -(glm::vec3(0.0f, 0.0f, 1.0f)) );
    }

    return anim;
}

glm::mat4 spinObj2(glm::mat4 anim, bool state) {
    float angle = -1.0f;

    if(state == true) {
    // anim = glm::translate(anim, glm::vec3(0.0f, 0.0f, 1.0f) );
        anim = glm::rotate(anim, glm::radians(angle), glm::vec3(0.0f, 0.0f, 2.0f));
    // anim = glm::translate(anim, -(glm::vec3(0.0f, 0.0f, 1.0f)) );
    }

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

void createAnim(GLuint shaderProgram, glm::mat4 anim) {
  const char* uniformName = "anim";
  GLint uniformAnim = glGetUniformLocation(shaderProgram, uniformName);
  if (uniformAnim == -1) {
      fprintf(stderr, "Error: could not bind uniform %s\n", uniformName);
      exit(EXIT_FAILURE);
  }
  glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
}

void printCube(GLfloat* vtx) {
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

    int vtxSize = 6*36;
    int arraySize = 4;
    static Cube cube[] ={
        Cube(MIDDLE, 0.0f),
        Cube(RIGHT, 0.0f),
        Cube(BOTTOM, 0.0f),
        Cube(TOP, 0.0f)
        // Cube(BOTTOM, 0.0f)
    };

    GLfloat* vtxArray[arraySize];
    //std::vector<GLfloat> vtxArray;

    GLuint VAOArray[arraySize];
    GLuint VBOArray[arraySize];


    for(int i = 0; i < arraySize; i++) {
        //GLfloat vtx[3*36];

        //std::copy(std::begin(initCube), std::end(initCube), std::begin(vtx));
        // printf("pointer: %u: \n", *cube[i].createCubes());
        vtxArray[i] = cube[i].createCubes();
        printf("Pointer then: %u \n", *vtxArray[i]);
        std::cout << vtxArray[i] << std::endl;
        printCube(vtxArray[i]);

        /* create and bind one Vertex Array Object */
        GLuint myVAO;
        glGenVertexArrays(1, &myVAO);
        glBindVertexArray(myVAO);

        VAOArray[i] = myVAO;

        /* generate and bind one Vertex Buffer Object */
        GLuint myVBO;
        glGenBuffers(1, &myVBO);
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);
        VBOArray[i] = myVBO;

        printf("counter %u \n", i);
    }

    for(int i = 0; i < arraySize; i++){
      printf("------------------------- \n");
      printCube(vtxArray[i]);
    }


    /* copy the vertex data to it */


    /* OpenGL settings */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* define and compile the vertex shader */
    const char* vertexShaderSource = GLSL(
      in vec3 position;
      //in vec2 textureCoordIn;
      in vec3 colorVtxIn;
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
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(GLfloat), 0);

    attributeName = "colorVtxIn";
    GLint colAttrib = glGetAttribLocation(shaderProgram, attributeName);
    if (colAttrib == -1) {
        fprintf(stderr, "Error: could not bind attribute %s\n", attributeName);
    }
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

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

    glm::mat4 anim2 = glm::mat4(1.0f);

    glm::mat4 anim3 = glm::mat4(1.0f);

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

    bool state = true;
    glm::mat4 bAnim;
    while (!glfwWindowShouldClose(myWindow)) {
        /* set the window background to black */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* make the object spin around */

        // anim = glm::rotate(anim, glm::radians(0.5f),
        //  glm::vec3(0.0f, 0.0f, 1.0f));
        /*GLint rnd = rand() % 3;
         if (rnd == 0) {
         anim = glm::rotate(anim, glm::radians(0.5f),
         glm::vec3(0.0f, 0.0f, 1.0f));
         }
         else if (rnd == 1) {
         anim = glm::rotate(anim, glm::radians(0.5f),
         glm::vec3(0.0f, 1.0f, 0.0f));
         }
         else if (rnd == 2) {
         anim = glm::rotate(anim, glm::radians(0.5f),
         glm::vec3(1.0f, 0.0f, 0.0f));
         }*/
        // glBufferData(GL_ARRAY_BUFFER, vtxSize*4, vtx2, GL_STATIC_DRAW);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        for(int i = 0; i < arraySize; i+=1) {
            createAnim(shaderProgram, anim);
            // if(i == 0) {
            //     createAnim(shaderProgram, anim2);
            //     anim2 = spinObj(anim2, state);
            // }
            // if(i == 1) {
            //     createAnim(shaderProgram, anim3);
            //     anim3 = spinObj2(anim3, state);
            // }

            glBufferData(GL_ARRAY_BUFFER, 6*36*4, vtxArray[i], GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //anim = handleCube(vtxArray, VAOArray, VBOArray, anim, arraySize, state);
        // state = false;
        //glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));


        // if(state == true) {
        //     anim = spinObj(anim, state);
        // }
        // state = false;

        // glUniformMatrix4fv(uniformAnim, 1, GL_FALSE, glm::value_ptr(anim));
        deltaTime = GLfloat(glfwGetTime() - lastTime);
        lastTime = deltaTime;

        look = getDirectionRightUp(myWindow, position, horizontalAngle, verticalAngle, initialFoV, speed, mouseSpeed, deltaTime);
        view = glm::lookAt(look[0], look[1], look[2]);

        /* draw the VAO */

        // glBufferData(GL_ARRAY_BUFFER, vtxSize*4, vtx, GL_STATIC_DRAW);

        // glDrawArrays(GL_TRIANGLES, 0, 36);


        // glBufferData(GL_ARRAY_BUFFER, vtxSize2*4, vtx2, GL_STATIC_DRAW);

        // glDrawArrays(GL_TRIANGLES, 0, 12*36);

        /* Swap buffers */
        glfwSwapBuffers(myWindow);

        /* poll events */
        glfwPollEvents();
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
        glDeleteBuffers(1, &VBOArray[i]);

        glDeleteVertexArrays(1, &VAOArray[i]);
    }



    /*                                                                        */
    /* termination of GLFW                                                    */
    /*                                                                        */
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
