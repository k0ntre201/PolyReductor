#version 430 core

uniform sampler2D opacityMap;
uniform bool hasOpacity;

in vec2 inTexCoord;

out vec4 color;

void main()
{
	if(hasOpacity)
	{
		vec4 temp = texture(opacityMap,inTexCoord);
		if(temp.a>0.1)
			color = vec4(0.0f);
	}
	else
	{
		color = vec4(0.0f);
	}
	//gl_FragDepth = gl_FragCoord.z;
	//color = vec4(0.0f);
}
