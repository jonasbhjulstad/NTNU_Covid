#version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

// Instanced attributes
layout (location = 4) in vec3 instancePos;
// layout (location = 5) in vec3 instanceRot;
layout (location = 5) in float instanceScale;
// layout (location = 7) in int instanceTexIndex;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 modelview;
	vec4 lightPos;
} ubo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

void main() 
{
	outColor = inColor;
	outUV = vec3(inUV, .0);
	
	vec4 locPos = vec4(inPos.xyz, 1.0);
	vec4 pos = vec4((locPos.xyz) + instancePos, 1.0);

	gl_Position = ubo.projection * ubo.modelview * pos;// * gRotMat * pos;
	outNormal = mat3(ubo.modelview) * inNormal;

	pos = ubo.modelview * vec4(inPos.xyz + instancePos, 1.0);
	vec3 lPos = mat3(ubo.modelview) * ubo.lightPos.xyz;
	outLightVec = lPos - pos.xyz;
	outViewVec = -pos.xyz;		
}
