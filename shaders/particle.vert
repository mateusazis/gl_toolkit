#version 400

layout(location = 0) in vec4 vp;
layout(location = 1) in vec2 uvIN;
layout(location = 2) in vec4 vColorIn;

uniform float radius;

out vec2 uv;
out vec4 vColorOut;

void main () {
  uv = uvIN;
  vec2 pos = vp.xy;
  pos.x = mix(-1.0, 1.0, vp.x/400.0);
  pos.y = mix(-1.0, 1.0, vp.y/400.0);

  vColorOut = vColorIn;
  
  gl_Position = vec4(pos, .0, 1.);
  gl_PointSize = 2. * radius;
}