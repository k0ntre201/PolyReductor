#version 430 core

const float PI = 3.14159265358979;

in vec2 texCoord;
in vec3 normal;
in vec4 position;
in vec4 shadowTexCoord;

uniform sampler2D shadowMap;
uniform sampler2D pajak;
uniform sampler2D papuga;

uniform samplerCube skyBox;

uniform vec3 lightPos = vec3(1.0f);
uniform vec3 eyePos;// = vec3(1.0f);

uniform int samplerCount = 0;

out vec4 color;


float shadowCalculation(vec4 shadowTexCoord, vec3 N)
{
	vec3 projectionCoords = shadowTexCoord.xyz/shadowTexCoord.w;
	projectionCoords = projectionCoords * 0.5 + 0.5;
	float shadow = 0.0;
	if(projectionCoords.z <= 1.0)
	{
		float closestDepth = texture(shadowMap,projectionCoords.xy).r;
		float currentDepth = projectionCoords.z;
		float bias = max(0.5 * (1.0 - dot(N,normalize(lightPos - position.xyz))), 0.005);
	
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1;y <= 1; ++y)
			{
				float pcfDepth = texture(shadowMap,projectionCoords.xy+vec2(x, y) * texelSize ).r;
				shadow+= currentDepth - bias > pcfDepth ? 1.0:0.0;
			}
		}
		shadow/=9.0;
	}
	return shadow;
}


float radicalInverse_VdC(in uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hamerslley(in uint i, in uint n)
{
	return vec2(float(i)/float(n),radicalInverse_VdC(i));
}

vec3 sphericalToCartesian(in vec2 s)
{
	/*float sinfi = sin(s.y);
	return vec3(cos(s.x)*sinfi,sin(s.x)*sinfi,sqrt(1-sinfi*sinfi));*/
	float phi = s.y*2.0*PI;
	float cosTheta = 1.0 - s.x;
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	return vec3(cos(phi)*sinTheta,sin(phi)*sinTheta,cosTheta);
}

vec3 shlick(in vec3 F0, in vec3 F90, in float m)
{
	return (F0 + (F90 - F0)*pow(1.0-m,5.0));
}

float fd(in float NdotL, in float NdotV, in float LdotH, in float roughness)
{
	float eb = clamp(0.0,0.5,roughness);
	float ef = clamp(1.0, 1.0/1.51, roughness);
	vec3 f0 = vec3(1.0,1.0,1.0);
	float f90 = eb + 2.0*LdotH*LdotH*roughness;
	vec3 ls = shlick(f0,vec3(f90),NdotL);
	vec3 vs = shlick(f0,vec3(f90),NdotV);
	
	return ls.r*vs.r*ef;
}

float NDFGGX(in float NdotH, in float roughness)
{
	float r2 = roughness*roughness;
	float div = (((NdotH*r2) - NdotH)*NdotH + 1.0);
	return r2/(div*div);
}

float halfGeometry(in float m, in float roughness)
{
	return (m*sqrt((-m*roughness*roughness)+m)*m + roughness);
}


float GeometryGGX(in float NdotL, in float NdotV, in float roughness)
{
	return 0.5/(halfGeometry(NdotL,roughness) + halfGeometry(NdotV,roughness));
}



void main()
{
	vec3 F0 = vec3(0.015);
	float roughness = 0.96;
	vec4 c1 = texture2D(pajak,texCoord);
	vec4 c2 = texture2D(papuga,texCoord);
	vec4 c = mix(c1,c2,0.5);
	vec3 N = normalize(normal);
	if(!gl_FrontFacing)
		N = -N;
	float NdotV = abs(dot(N,normalize(eyePos - position.xyz))) + 10e-6;
	int count = 0;
	vec3 diffAndSpec = vec3(0.0);
	for(int i=0;i<samplerCount;i++)
	{
		vec2 u = hamerslley(i,samplerCount);
		vec3 L = normalize(sphericalToCartesian(u));
		float NdotL = dot(N,L);
		vec4 lightColor = texture(skyBox,L);
		if(NdotL>0)
		{
			vec3 H=normalize(L+normalize(eyePos-position.xyz));
			float HdotL = min(dot(H,L), 1.0);
			float HdotN = min(dot(H,N), 1.0);
			float HdotV = min(dot(H,normalize(eyePos-position.xyz)), 1.0);
			float G = GeometryGGX(NdotL,NdotL,roughness);
			float D = NDFGGX(HdotN,HdotL);
			vec3 Fd0 = shlick(F0,vec3(1.0),NdotL);
			//float m = roughness;
			//float Fd0 = F0.r+(1-pow(1-HdotV,5)*(1-F0.r));
			//float G = min(1,min(2*HdotN*NdotV/HdotV,2*HdotN*NdotL/HdotV));
			//float D = exp((pow(HdotN,2)-1)/(m*m*pow(HdotN,2)))/(pow(HdotN,4)*m*m);
			//diffAndSpec+=lightColor.rgb*NdotL;// + Fd0*D*G/(4*NdotL*NdotV);//*(fd(NdotL,NdotV,HdotL,roughness)));//+Fd0*D*G/(4*NdotL*NdotV));
			diffAndSpec+=lightColor.rgb*c.rgb*fd(NdotL,NdotV,HdotL,roughness);// + (lightColor.rgb*Fd0*D*G/(4.0*NdotV*NdotL));
		count++;
		}
		else
		{
			diffAndSpec-=NdotL*0.5*c2.rgb*lightColor.rgb;
			count++;
		}
	}
	if(count>0)
		color=vec4(diffAndSpec/count, 1.0);
	//color = normalize(dot(normal,normalize(L))) * vec4(1.0,0.0,1.0,0.0);
	
	float s = shadowCalculation(shadowTexCoord, N);
	
	if(dot(N,lightPos)>0)
		color += dot(N,normalize(lightPos - position.xyz))*c1;
	else
		color += abs(dot(N,normalize(lightPos - position.xyz)))*c1*c2;
}

















