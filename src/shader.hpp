#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader {

private:

  unsigned int ID;

public:

  Shader(const char* vertexPath, const char* fragmentPath) {
    
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile, fragmentFile;
    std::stringstream vertexStream, fragmentStream;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vertexFile.open(vertexPath);
      fragmentFile.open(fragmentPath);

      vertexStream << vertexFile.rdbuf();
      fragmentStream << fragmentFile.rdbuf();

      vertexFile.close();
      fragmentFile.close();

      vertexCode = vertexStream.str();
      fragmentCode = fragmentStream.str();

    } catch(std::ifstream::failure e) {
      std::cout << "Failed reading files: " << e.code().message() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }    

    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }    

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  void use() {
    glUseProgram(ID);
  }

  void setFloat(const char* uniform, float value) {
    unsigned int location = glGetUniformLocation(ID, uniform);
    glUniform1f(location, value);
  }

  void setTransformationMatrix(const char* uniform, glm::mat4 value) {

    unsigned int location = glGetUniformLocation(ID, uniform);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

  }
};
