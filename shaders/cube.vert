#version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

// Instanced attributes
layout (location = 4) in vec3 instancePos;
// layout (location = 5) in vec3 instanceRot;
layout (location = 5) in vec3 instanceScale;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 modelview;
	vec4 lightPos;
} ubo;

layout (binding = 1) uniform UBO_Light
{
	vec4 albedo;
	vec4 ambientLightColor;
} ubo_light;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;


void main() 
{
	mat3 rot = mat3(1.0);
	//compute vertex position with scaling
	outUV = vec3(inUV, .0);
	
	vec3 locPos = vec3(inPos);
	locPos.x *= instanceScale.x;
	locPos.y *= instanceScale.y;
	locPos.z *= instanceScale.z;
	vec4 worldPos = vec4(rot*locPos + instancePos, 1.0);
	gl_Position = ubo.projection*ubo.modelview*worldPos;

	vec3 lightColor = ubo_light.albedo.xyz*ubo_light.albedo.w;
	vec3 ambientLight = ubo_light.ambientLightColor.xyz*ubo_light.ambientLightColor.w;
	vec3 directionToLight = ubo.lightPos.xyz - worldPos.xyz;
	vec3 diffuseLight = lightColor*max(dot(inNormal, normalize(directionToLight)), 0);
	outColor = (diffuseLight + ambientLight)*inColor;
	// gl_Position = ubo.projection * ubo.modelview * pos;
	outNormal = mat3(ubo.modelview) * inNormal;

	vec3 lPos = mat3(ubo.modelview) * ubo.lightPos.xyz;
	outLightVec = directionToLight;
	outViewVec = -worldPos.xyz;		
}



