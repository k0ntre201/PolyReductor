#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 MVP;
uniform mat3 normalMatrix;

out vec3 position;
out vec3 normal;

void main()
{
	vec4 p = MVP*vec4(inPosition,1.0);
	position = p.xyz;
	normal = normalize(normalMatrix*inNormal);
	gl_Position = p;
}
