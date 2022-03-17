#version 450
precision mediump float;
layout(location = 0) in float radius;
layout(location = 0) out vec4 outColor;

void main() {

    float dist = distance(gl_FragCoord.xy, vec2(0, 0));
  if (dist > radius)
  {
      discard;
  }
outColor = vec4(1.0,1.0,.4, 1.0);

}
