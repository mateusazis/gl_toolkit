#version 400

layout(location = 0) in vec4 vp;
out vec4 vp_out;

void main () {
  vp_out = vp;
  
  gl_Position = vp;
}