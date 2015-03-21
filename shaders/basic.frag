#version 400

out vec4 frag_colour;
in vec4 vp_out;

void main () {
  frag_colour = vec4 (vp_out.xy, 0.0, 1.0);
}