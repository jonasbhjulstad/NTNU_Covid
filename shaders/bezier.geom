#version 450

/* \brief Geometry GLSL shader that demonstrates how to draw basic thick and smooth bezier curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;
const int SegmentsMax = 30; // max_vertices = (SegmentsMax+1)*4;
const int SegmentsMin = 3; // min number of segments per curve


layout(binding = 0) uniform UBO
{
    vec3 P1scale;
    vec3 P2scale;
};

layout(location = 0) in vec4 inColor[];

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec4 outVertex;
vec2 toScreenSpace(vec4 vertex)
{
    return vec2( vertex.xy / vertex.w );
}

float toZValue(vec4 vertex)
{
    return (vertex.z/vertex.w);
}

vec4 toBezier(float delta, int i, vec4 P0, vec4 P1, vec4 P2, vec4 P3)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

const int N_vertices = 10;
void main(void)
{
    vec3 P0 = vec3(gl_in[0].gl_Position);
    vec3 P3 = vec3(gl_in[1].gl_Position);
    float dist = distance(P0, P3);
    for (int i = 0; i < N_vertices; i++)
    {
        float t = 1./N_vertices*i;
        outColor = t*(inColor[1]-inColor[0]) + inColor[0];
        gl_Position = vec4(pow(1-t, 3)*P0 + 3*pow(1-t, 2)*t*(P1scale*dist) + 3*(1-t)*pow(t,2)*(P2scale*dist) + pow(t, 3)*P3, 1.0);
        EmitVertex();
    }

}
