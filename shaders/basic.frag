#version 400

out vec4 frag_colour;
in vec2 uv;

uniform sampler2D tex;

void main () {
  //frag_colour = vec4 (vp_out.xy, 0.0, 1.0);
  frag_colour = vec4(1.0 - texture2D(tex, uv).rgb, 1.0);
  //frag_colour = vec4(uv, 0., 1.);
}