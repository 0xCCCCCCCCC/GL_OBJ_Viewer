//
//  shader.h
//  Ray_Tracing
//
//  Created by YNK on 2022/7/25.
//

#ifndef shader_h
#define shader_h

#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"

typedef GLuint* Shader;

char *readShaderFile(const char *fn);

void shaderGenShader(const char *vertexShaderFile,
                     const char *fragmentShaderFile, unsigned int *vertexShader,
                     unsigned int *fragmentShader);

void shaderAttachShader(unsigned int *programHandle, unsigned int *vertexShader,
                        unsigned int *fragmentShader);

void shaderDeleteShader(unsigned int *vertexShader, unsigned int *fragmentShader);

Shader shaderCreate(const char *vertexShaderFile, const char *fragmentShaderFile);

void shaderUse(Shader shader);

void shaderSetMat4(Shader shader, std::string name, glm::mat4 mat);

void shaderSetVec3(Shader shader, std::string name, glm::vec3 vec);

void shaderSetFloat(Shader shader, std::string name, float val);

#endif /* shader_h */
