#version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 model;
	vec4 lightPos;
	float nodeRotSpeed;
} ubo;

// Instanced attributes
layout (location = 3) in vec3 instancePos;
layout (location = 4) in vec3 instanceRot;


layout (location = 0) out vec3 outColor;

void main() 
{
	outColor = inColor;

	mat3 mx, my, mz;


	//Rotation x
	float s = sin(instanceRot.x);
	float c = cos(instanceRot.x);

	mx[0] = vec3(c, s, .0);
	mx[1] = vec3(-s, c, .0);
	mx[2] = vec3(.0,.0, 1.0);

	//Rotation y
	s = sin(instanceRot.y + ubo.nodeRotSpeed);
	c = cos(instanceRot.y + ubo.nodeRotSpeed);

	my[0] = vec3(c, .0, s);
	my[1] = vec3(.0, 1.0, .0);
	my[2] = vec3(-s, .0, c);

	//Rotation z

	s = sin(instanceRot.z);
	c = cos(instanceRot.z);

	mz[0] = vec3(1.0, .0,.0);
	mz[1] = vec3(.0,c,s);
	mz[2] = vec3(.0,-s,c);

	mat3 rotMat = mz*my*mx;

	mat4 gRotMat;
	gRotMat[0] = vec4(rotMat[0], .0);
	gRotMat[1] = vec4(rotMat[1], .0);
	gRotMat[2] = vec4(rotMat[2], .0);
	gRotMat[3] = vec4(.0,.0,.0, 1.0);
	// gl_Position = ubo.projection * gRotMat * pos;
	// gl_Position = vec4(instancePos, 1.0);
	gl_Position = ubo.projection * ubo.model*(vec4(rotMat*inPos.xyz, 1.0) + vec4(instancePos.xyz, 1.0));
}
