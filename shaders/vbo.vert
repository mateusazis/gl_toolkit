#version 400

layout(location = 0) in vec4 vp;
layout(location = 2) in vec4 color;

out vec4 fColor;

void main () {
  gl_Position = vp;
  fColor = color;
}