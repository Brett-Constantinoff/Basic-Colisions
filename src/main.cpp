#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <fstream>
#include <irrKlang/include/irrKlang.h>
#include <unistd.h>
#include <cstdlib>
#include "shader.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

void moveObject(GLFWwindow *win, glm::mat4 *model, glm::vec3 *position, glm::vec3 *color, irrklang::ISoundEngine* soundEngine, bool* playSound);

int main(){

    /*--INIT APPLICATION STATES--*/
    bool playSound = true;

    /*--INIT--*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //required for apple users
#endif

    const unsigned int width = 800, height = 800;
    GLFWwindow *win = glfwCreateWindow(width, height, "Basic Collisions", NULL, NULL);
    if(!win){
        std::cerr << "ERROR CREATING OPENGL WINDOW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);
    glViewport(0, 0, width, height);

    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        std::cerr << "ERROR INITIALIZING GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    /*--SOUND SETUP--*/
    irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    if(!soundEngine){
        std::cout << "ERROR INIT SOUND ENGINE" << std::endl;
        exit(EXIT_FAILURE);
    }

    /*--SHADER SETUP--*/
    Shader square1("/Users/brettconstantinoff/Desktop/opengl/BasicCollision/shaders/square1.glsl"); //create shader file for cube 1
   
    /*BUFFER-CREATION*/
    float square1Vert[]{
        0.1f,  0.1f, 0.0f,  // top right
        0.1f, -0.1f, 0.0f,  // bottom right
        -0.1f, -0.1f, 0.0f,  // bottom left
        -0.1f,  0.1f, 0.0f   // top le
    };
    unsigned int square1Ind[]{
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //vertex buffer creation
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square1Vert), square1Vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    //index buffer creation
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square1Ind), square1Ind, GL_STATIC_DRAW);
 
    glm::mat4 model = glm::mat4(1.0f);
    
    /*--RENDER LOOP--*/
    while(!glfwWindowShouldClose(win)){
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 squareColor = glm::vec3(0.50f, 0.50f, 0.50f);

        model = glm::translate(model, position);
        moveObject(win, &model, &position, &squareColor, soundEngine, &playSound);
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(square1.ID);
        glBindVertexArray(vao);
    
        glUniformMatrix4fv(glGetUniformLocation(square1.ID, "model"), 1, GL_FALSE, &model[0][0]);
        glUniform3fv(glGetUniformLocation(square1.ID, "uColor"), 1, &squareColor[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    return 0;
}

void moveObject(GLFWwindow *win, glm::mat4 *model,  glm::vec3 *position, glm::vec3 *color, irrklang::ISoundEngine* soundEngine, bool* playSound){

    *position = *model * glm::vec4(*position, 1.0f);
    
    if(glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS){
        if((*position).y + 0.1 <= 1.0f){
             *model = glm::translate(*model, glm::vec3(0.0f, 0.05f, 0.0f));
             *playSound = true;
        }
        else{
            std::cout << "Collision with upper screen bound" << std::endl;
            *color = glm::vec3(1.0f, 0.0f, 0.0f);
            if(*playSound){
                soundEngine->play2D("/Users/brettconstantinoff/Desktop/opengl/BasicCollision/sounds/clack.wav", false);
                *playSound = false;
            }
        }
    }
    if(glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS){
        if((*position).y - 0.1 >= -1.0f){
            *model = glm::translate(*model, glm::vec3(0.0f, -0.05f, 0.0f));
            *playSound = true;
        }
        else{
            std::cout << "Collision with lower screen bound" << std::endl;
            *color = glm::vec3(1.0f, 0.0f, 0.0f);
            if(*playSound){
                soundEngine->play2D("/Users/brettconstantinoff/Desktop/opengl/BasicCollision/sounds/clack.wav", false);
                *playSound = false;
            }
        }
    }
    if(glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS){
        if((*position).x + 0.1f <= 1.0f){
            *model = glm::translate(*model, glm::vec3(0.05f, 0.0f, 0.0f));
            *playSound = true;
        }
        else{
            std::cout << "Collision with right screen bound" << std::endl;
            *color = glm::vec3(1.0f, 0.0f, 0.0f);
            if(*playSound){
                soundEngine->play2D("/Users/brettconstantinoff/Desktop/opengl/BasicCollision/sounds/clack.wav", false);
                *playSound = false;
            }
        }
    }
    if(glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS){
        if((*position).x - 0.1f >= -1.0f){
            *model = glm::translate(*model, glm::vec3(-0.05f, 0.0f, 0.0f));
            *playSound = true;
        }
        else{
            std::cout << "Collision with left screen bound" << std::endl;
            *color = glm::vec3(1.0f, 0.0f, 0.0f);
            if(*playSound){
                soundEngine->play2D("/Users/brettconstantinoff/Desktop/opengl/BasicCollision/sounds/clack.wav", false);
                *playSound = false;
            }
        }
    }
}

