#version 450

// layout (binding = 1) uniform sampler2DArray samplerArray;

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inUV;
layout (location = 3) in vec3 inViewVec;
layout (location = 4) in vec3 inLightVec;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	// vec4 color = texture(samplerArray, inUV) * vec4(inColor, 1.0);	
	// vec3 N = normalize(inNormal);
	// vec3 L = normalize(inLightVec);
	// vec3 V = normalize(inViewVec);
	// vec3 R = reflect(-L, N);
	// vec3 diffuse = max(dot(N, L), 0.1) * inColor;
	// outFragColor = vec4(diffuse * color.rgb, 1.0);		
	outFragColor = vec4(inColor, 1.0);
}