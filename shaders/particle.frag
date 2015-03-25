#version 400

out vec4 frag_colour;
in vec4 vColorOut;

uniform sampler2D tex;

void main () {
	vec2 coord = (gl_PointCoord.xy - 0.5) / .5;
	float inside = float(length(coord) <= 1.0);
	frag_colour = vec4(vColorOut.rgb, inside);
}