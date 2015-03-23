#version 400

out vec4 frag_colour;
in vec2 uv;

uniform sampler2D tex;
uniform int size;
uniform float multiplier;

void main () {
	vec2 uvTemp = uv;
	//uvTemp.y = 1.0 - uvTemp.y;
	vec4 color = texture2D(tex, uvTemp);
	vec2 pos = gl_FragCoord.xy;
	int rest = int(pos.y) % (2*size);
	color.rgb = color.rgb * mix(1.0, multiplier, float(rest < size));
	frag_colour = color;
}