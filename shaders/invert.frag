#version 400

out vec4 frag_colour;
in vec2 uv;

uniform sampler2D tex;

void main () {
	vec2 uvTemp = uv;
	uvTemp.y = 1.0 - uvTemp.y;
	frag_colour = texture2D(tex, uvTemp);
}