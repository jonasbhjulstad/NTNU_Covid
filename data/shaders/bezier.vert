#version 450
#define M_PI 3.1415926535897932384626433832795
// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

// Instanced attributes
layout (location = 4) in vec3 startNodePos;
// layout (location = 5) in vec3 instanceRot;
layout (location = 5) in vec3 endNodePos;
layout(location = 6) in vec3 scale;
// layout (location = 7) in int instanceTexIndex;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 modelview;
	vec4 lightPos;
	float theta;
} ubo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outUV;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;


mat3 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

void main() 
{
	outColor = inColor;
	outUV = vec3(inUV, .0);


	vec3 bezierDirection = endNodePos - startNodePos;

	float theta = acos(dot(bezierDirection,vec3(.0,.0,1.0))/(distance(bezierDirection, vec3(.0,.0,.0))));
	vec3 u = cross(bezierDirection, vec3(.0,.0,1.0));

	vec4 centerPos = vec4((endNodePos - startNodePos)/2 + startNodePos, 1.0);
	mat3 rotMat = rotationMatrix(u, theta);
	
	vec4 locPos = vec4(inPos.xyz*scale, 1.0);
	locPos.z = locPos.z*abs(distance(startNodePos, endNodePos))/2;
	locPos.x *=.01;
	locPos.y *=.01;
	vec4 pos = ubo.modelview*vec4(rotMat*locPos.xyz + centerPos.xyz, 1.0);
	gl_Position = ubo.projection * pos;
	outNormal = mat3(ubo.modelview) * inNormal;

	vec3 lPos = mat3(ubo.modelview) * ubo.lightPos.xyz;
	outLightVec = lPos - pos.xyz;
	outViewVec = -pos.xyz;		
}
