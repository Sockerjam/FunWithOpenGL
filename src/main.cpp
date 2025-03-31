#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "shader.hpp"
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include "camera.hpp"

const int FPS = 60;
const float TIME_MS_FRAME = 1000.0 / FPS;
Camera camera;

void setViewPort(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.navigateLeft(deltaTime);
    return;
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.navigateRight(deltaTime);
    return;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.navigateBackwards(deltaTime);
    return;
  } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.navigateForward(deltaTime);
    return;
  }
}

glm::mat4 modelMatrix(float time) {
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0, 0, -3));
  model = glm::rotate(model, glm::radians(time * 20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  return model;
}

glm::mat4 viewMatrix() {
  glm::mat4 view = glm::mat4(1.0);
  return view;
}

glm::mat4 projectionMatrix() {
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
  return projection;
}


int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Error creating window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  glfwSetFramebufferSizeCallback(window, setViewPort);

  Shader shader("./src/shaders/vertex.vert", "./src/shaders/fragment.frag");

  float vertices[] = {
    -0.5, 0.5, 0.0,
    0.5, 0.5, 0.0,
    0.5, -0.5, 0.0,
    -0.5, -0.5, 0.0,

    -0.5, 0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, -0.5, -0.5,
    -0.5, -0.5, -0.5,
    
  };

  float colors[] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0,

    1.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    0.0, 0.0, 1.0,
    1.0, 0.5, 0.5
  };

  unsigned int indicies[] = {
    0, 2, 3, // front
    0, 1, 2, // front

    4, 6, 7, // back
    4, 5, 6, // back

    7, 2, 3, // bottom
    7, 6, 2, // bottom

    0, 5, 4, // top
    0, 1, 5, // top
    
    4, 0, 7, // left side
    0, 7, 3, // left side

    5, 1, 2, // right side
    5, 6, 2 // right side

  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  unsigned int VBO, CBO, EBO, VAO;

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &CBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


  float deltaTime = 0;
  float msPreviousFrame = 0;

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // Set the callback with a lambda that retrieves the camera instance
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    camera.mouse_callback(window, xpos, ypos);  // Ensure this is a non-static member function
  });

  while (!glfwWindowShouldClose(window)) {
  
    int startTime = glfwGetTime() * 1000.0f;

    float time = glfwGetTime(); 
    processInput(window, camera, deltaTime);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 1.0, 0.0));

    // shader.setTransformationMatrix("model", modelMatrix(time));
    shader.setTransformationMatrix("view", camera.getCamera());
    shader.setTransformationMatrix("projection", projectionMatrix());

    glBindVertexArray(VAO);

    for(int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(cubePositions[i]));

      if (i % 3 == 0) {
        float animation = sin(time) / 2 + 0.5;
        shader.setFloat("time", animation);
        model = glm::rotate(model, glm::radians(time * 20), glm::vec3(1.0, 0.3, 0.5));
      } else {
        float animation = sin(time * 1.5) / 2 + 0.5;
        shader.setFloat("time", animation);
        model = glm::rotate(model, glm::radians(-time * 20), glm::vec3(1.0, 0.3, 0.0));
      }

      shader.setTransformationMatrix("model", model);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    int endTime = glfwGetTime() * 1000.0f;
    int difference = endTime - startTime;
    int delayTime = TIME_MS_FRAME - difference;

    if (delayTime > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
    }

    deltaTime = ((glfwGetTime() * 1000.0f) - msPreviousFrame) / 1000.0f;

    msPreviousFrame = glfwGetTime() * 1000.0f;
    
  }

  glfwTerminate();
  return 0;
}
