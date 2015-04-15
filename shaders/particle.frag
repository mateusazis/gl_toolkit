#version 400

out vec4 frag_colour;
in vec4 vColorOut;
in float angleOUT;

uniform sampler2D tex;

const float DEG_2_RAD = 3.14 / 180.0;
const float TWO_PI = 2 * 3.14;

void main () {
	vec2 coord = (gl_PointCoord.xy - 0.5) / .5;
	float inside = float(length(coord) <= 1.0);
	vec4 c = vec4(vColorOut.rgb, inside);
	
	float angle = atan(coord.y, coord.x) + 3.14;
	angle += angleOUT;

	
	angle -= TWO_PI * float(angle > TWO_PI);
	const float MIN_ANGLE = 15 * DEG_2_RAD;
	bool drawBlankLine = angle < MIN_ANGLE || angle >= TWO_PI - MIN_ANGLE;
	vec4 white = vec4(vec3(1),inside);



	/*angle = atan(coord.y, coord.x) / TWO_PI;
	angle += 3.14/2.0;
	white = vec4(vec3(angle), 1.0);
	drawBlankLine = true;*/



	c = mix(c, white, float(drawBlankLine));

	frag_colour = c;
}