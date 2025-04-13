#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>
#include <ios>
#include <iostream>

class Camera {

private:
  bool firstMouse = true;
  float lastX = 400;
  float lastY = 300;
  float yaw = 0;
  float pitch = 0;
  glm::vec3 eye = glm::vec3(0, 0, 3);
  glm::vec3 direction = glm::vec3(0, 0, -1);
  glm::vec3 up = glm::vec3(0, 1, 0);

  glm::mat4 view = glm::mat4(1);

public:

  Camera() = default;

  void navigateForward(float deltaTime) {
    eye += direction * deltaTime;
  }

  void navigateBackwards(float deltaTime) {
    eye -= direction * deltaTime;
  }

  void navigateRight(float deltaTime) {
    glm::vec3 rightVector = glm::normalize(glm::cross(direction, up));
    eye += rightVector * deltaTime;
  }

  void navigateLeft(float deltaTime) {
    glm::vec3 rightVector = glm::normalize(glm::cross(direction, up));
    eye -= rightVector * deltaTime;
  }

  void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89) {
      pitch = 89;
    }
    if (pitch < -89) {
      pitch = -89;
    }

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    this->direction = glm::normalize(direction);
  }

  glm::mat4 getCamera() {
    glm::vec3 target = eye + direction;
    glm::vec3 directionVector = glm::normalize(eye - target);
    glm::vec3 rightVector = glm::cross(glm::normalize(up), directionVector);
    glm::vec3 upVector = glm::cross(directionVector, rightVector);

    glm::mat4 rotation = glm::mat4(1.0);
    rotation[0] = glm::vec4(rightVector, 0);
    rotation[1] = glm::vec4(upVector, 0);
    rotation[2] = glm::vec4(directionVector, 0);

    glm::mat4 translate = glm::mat4(1.0);

    translate = glm::translate(translate, -eye);

    glm::mat4 lookAtMatrix = glm::transpose(rotation) * translate;

    view = lookAtMatrix;

    // view = glm::lookAt(eye, eye + direction, up);
    return view;
  }
};
