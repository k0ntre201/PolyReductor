#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 inTexCoord;

uniform mat4 shadowTextureMatrix;

void main()
{
	gl_Position = shadowTextureMatrix*vec4(position,1.0f);
	inTexCoord = texCoord;
}
