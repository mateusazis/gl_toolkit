#version 400

out vec4 frag_colour;
in vec4 fColor;

uniform sampler2D tex;

void main () {
  frag_colour = fColor;
}