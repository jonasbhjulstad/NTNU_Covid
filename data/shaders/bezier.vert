#version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

// Instanced attributes
layout (location = 4) in vec3 startNodePos;
// layout (location = 5) in vec3 instanceRot;
layout (location = 5) in vec3 endNodePos;
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


	vec3 bezierDirection = normalize(endNodePos - startNodePos);
	vec3 u = cross(bezierDirection, vec3(0,0,1));
	float theta = asin(float(abs(u)/(2.)));


	vec3 centerPos = (endNodePos - startNodePos)/2 + startNodePos;
	mat3 rotMat;
	float c = cos(theta);
	float s = sin(theta);

	rotMat[0] = vec3(c + u.x*u.x*(1-c), u.x*u.y*(1-c) - u.z*s, u.x*u.z*(1-c) + u.y*s);
	rotMat[1] = vec3(u.y*u.x*(1-c) + u.z*s, c + u.y*u.y*(1-c), u.y*u.z*(1-c)-u.x*s);
	rotMat[2] = vec3(u.z*u.x*(1-c) - u.y*s, u.z*u.y*(1-c) + u.x*s, c + u.z*u.z*(1-c));

	// mat3 mx, my, mz;
	
	// // rotate around x
	// float s = sin(instanceRot.x);
	// float c = cos(instanceRot.x);

	// mx[0] = vec3(c, s, 0.0);
	// mx[1] = vec3(-s, c, 0.0);
	// mx[2] = vec3(0.0, 0.0, 1.0);
	
	// // rotate around y
	// s = sin(instanceRot.y);
	// c = cos(instanceRot.y);

	// my[0] = vec3(c, 0.0, s);
	// my[1] = vec3(0.0, 1.0, 0.0);
	// my[2] = vec3(-s, 0.0, c);
	
	// // rot around z
	// s = sin(instanceRot.z);
	// c = cos(instanceRot.z);	
	
	// mz[0] = vec3(1.0, 0.0, 0.0);
	// mz[1] = vec3(0.0, c, s);
	// mz[2] = vec3(0.0, -s, c);
	
	// mat3 rotMat = mz * my * mx;

	// mat4 gRotMat;
	// s = sin(instanceRot.y);
	// c = cos(instanceRot.y);
	// gRotMat[0] = vec4(c, 0.0, s, 0.0);
	// gRotMat[1] = vec4(0.0, 1.0, 0.0, 0.0);
	// gRotMat[2] = vec4(-s, 0.0, c, 0.0);
	// gRotMat[3] = vec4(0.0, 0.0, 0.0, 1.0);	
	
	vec4 locPos = vec4(inPos.xyz, 1.0);
	vec4 pos = vec4((locPos.xyz)*rotMat + centerPos, 1.0);

	gl_Position = ubo.projection * ubo.modelview * pos;// * gRotMat * pos;
	outNormal = mat3(ubo.modelview) * inNormal;

	pos = ubo.modelview * vec4(inPos.xyz*rotMat + centerPos, 1.0);
	vec3 lPos = mat3(ubo.modelview) * ubo.lightPos.xyz;
	outLightVec = lPos - pos.xyz;
	outViewVec = -pos.xyz;		
}
