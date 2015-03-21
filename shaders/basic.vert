#version 400

layout(location = 0) in vec4 vp;
layout(location = 1) in vec2 uvIN;

out vec2 uv;

void main () {
  uv = uvIN;
  
  gl_Position = vp;
}