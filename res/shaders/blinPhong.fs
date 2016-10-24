#version 450 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shiness;
};

in vec3 position;
in vec3 normal;

uniform Material material;
uniform vec3 lightPosition;
uniform vec3 eyePosition;

out vec4 color;

void main()
{
	vec3 lightDirection = normalize(lightPosition - position);
	vec3 eyeDirection = normalize(eyePosition - position);

	vec3 N;
	if(gl_FrontFacing == true)
		N = normal;
	else
		N = -normal;
	
	float NdotL = dot(lightDirection,N);
	color = vec4(material.ambient,1.0);
	if(NdotL > 0.0)
	{
		vec3 diffuse = material.diffuse * NdotL;
		vec3 H = normalize(eyeDirection + lightDirection);
		float NdotH = dot(N, H);
		//vec3 specular = material.specular*(pow(NdotH,material.shiness));
		color.rgb+=(diffuse);// + specular);
	}
}
