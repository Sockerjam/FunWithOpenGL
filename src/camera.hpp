#pragma once

#include <glm/glm.hpp>

class Camera {

private:

  glm::vec3 direction{0, 0, -3};

public:

  Camera() = default;

  void setDirection(glm::vec3 newDirection) {
    direction = newDirection;
  }

  glm::vec3 getDirection() {
    return direction;
  }

};
