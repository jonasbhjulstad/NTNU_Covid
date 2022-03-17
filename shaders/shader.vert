#version 450

precision mediump float;

layout(location = 0) in vec3 position;
layout(location = 1) in float radius;
layout(location = 0) out float outRadius;
void main() {
  gl_Position = vec4(position.xyz, 1.0);
  float outRadius = radius;
  gl_PointSize = 100.0;
}