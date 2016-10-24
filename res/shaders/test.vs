#version 430 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

uniform mat4 MVP;// = mat4(1.0);
uniform mat3 normalMatrix;
uniform mat4 shadowTextureMatrix = mat4(1.0);

out vec2 texCoord;
out vec3 normal;
out vec4 position;
out vec4 shadowTexCoord;

void main()
{
	position = MVP * vec4(inPosition,1.0);
	gl_Position = position;
	texCoord = inTexCoord;
	normal = normalMatrix*inNormal;
	//normal = inNormal;
	shadowTexCoord = shadowTextureMatrix*vec4(inPosition,1.0f);
}
