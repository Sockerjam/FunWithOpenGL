#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColor;

out vec4 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(vertPosition, 1);
  fragColor = vec4(vertColor, 1);
}
