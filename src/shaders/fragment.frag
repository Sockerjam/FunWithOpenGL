#version 330 core

in vec4 fragColor;
out vec4 outFragColor;

uniform float time;

void main() {
  outFragColor = vec4(fragColor.x + time + fragColor.y, fragColor.y * time + fragColor.z, fragColor.z - time + fragColor.x, fragColor.w);
}
