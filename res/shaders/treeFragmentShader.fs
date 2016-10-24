#version 450 core;

/*variables from vertex shader*/
in vec3 position;
in vec3 normal;
in vec3 textureCoordinates;
in vec3 tangent;
in vec3 bitangent;


/*uniform variables from program for leaves*/
uniform bool isLeaf;
uniform float leafRougness;
uniform float leafF0;
uniform sampler2D leafFront;
uniform sampler2D leafFrontTranslucecy;
uniform sampler2D leafFrontNormalMap;
uniform sampler2D leafBack;
uniform sampler2D leafBackTranslucecy;
uniform sampler2D leafBackNormalMap;
uniform sampler2D leafAlphaMask;
uniform sampler2D leafThicknessMap;

/*uniform variables for bark*/
uniform float barkRoughness;
uniform float barkF0;
uniform sampler2D barkDiffuseMap;
uniform sampler2D barkNormalMap;
uniform sampler2D barkSpecularMap;

/*uniform variables for everythink*/
uniform samplerCube cubeMap;
uniform sampler2DShadow shadowMap;
uniform vec3 lightPosition;
uniform vec3 eyePosition;



const int N = 256;

vec3 hammersleyPoints256[N] = (
     vec3(0.0, 0.0, 1.0),
     vec3(0.706894, 0.0173533, 0.707107),
     vec3(0.499398, 0.0245338, 0.866025),
     vec3(0.863679, 0.0637088, 0.5),
     vec3(0.351851, 0.0346543, 0.935414),
     vec3(0.784624, 0.0967741, 0.612372),
     vec3(0.605744, 0.0898537, 0.790569),
     vec3(0.921643, 0.15992, 0.353553),
     vec3(0.245196, 0.0487726, 0.968246),
     vec3(0.731777, 0.164326, 0.661438),
     vec3(0.542264, 0.13583, 0.829156),
     vec3(0.868736, 0.240412, 0.433013),
     vec3(0.414367, 0.125697, 0.901388),
     vec3(0.787307, 0.260091, 0.559017),
     vec3(0.622773, 0.222832, 0.75),
     vec3(0.903366, 0.348467, 0.25),
     vec3(0.16332, 0.0676496, 0.984251),
     vec3(0.666339, 0.295368, 0.684653),
     vec3(0.479413, 0.226745, 0.847791),
     vec3(0.789506, 0.397404, 0.467707),
     vec3(0.34861, 0.186336, 0.918559),
     vec3(0.704851, 0.399293, 0.586302),
     vec3(0.546697, 0.327677, 0.770552),
     vec3(0.804277, 0.509303, 0.306186),
     vec3(0.254584, 0.170108, 0.951972),
     vec3(0.629991, 0.44369, 0.637377),
     vec3(0.470922, 0.34926, 0.810093),
     vec3(0.724142, 0.565126, 0.395285),
     vec3(0.361543, 0.29671, 0.883883),
     vec3(0.641955, 0.553754, 0.53033),
     vec3(0.507295, 0.459785, 0.728869),
     vec3(0.712841, 0.678681, 0.176777),
     vec3(0.0883883, 0.0883883, 0.992157),
     vec3(0.495139, 0.52006, 0.695971),
     vec3(0.346114, 0.381877, 0.856957),
     vec3(0.571526, 0.662558, 0.484123),
     vec3(0.237898, 0.289879, 0.927025),
     vec3(0.492426, 0.630985, 0.599479),
     vec3(0.372312, 0.502005, 0.780625),
     vec3(0.543407, 0.771579, 0.330719),
     vec3(0.155287, 0.232403, 0.960143),
     vec3(0.406783, 0.64238, 0.649519),
     vec3(0.294489, 0.491326, 0.81968),
     vec3(0.448544, 0.791791, 0.414578),
     vec3(0.212456, 0.397477, 0.892679),
     vec3(0.377011, 0.748991, 0.544862),
     vec3(0.287807, 0.608516, 0.73951),
     vec3(0.395629, 0.892526, 0.216506),
     vec3(0.0828534, 0.200026, 0.976281),
     vec3(0.266146, 0.689957, 0.673146),
     vec3(0.183559, 0.513012, 0.838525),
     vec3(0.280017, 0.847623, 0.450694),
     vec3(0.120346, 0.396727, 0.910014),
     vec3(0.218619, 0.789988, 0.572822),
     vec3(0.15782, 0.630054, 0.760345),
     vec3(0.210369, 0.936814, 0.279509),
     vec3(0.06452, 0.324364, 0.943729),
     vec3(0.133457, 0.769132, 0.625),
     vec3(0.0879618, 0.59299, 0.800391),
     vec3(0.113478, 0.920053, 0.375),
     vec3(0.0474523, 0.481792, 0.875),
     vec3(0.0630416, 0.854635, 0.515388),
     vec3(0.0341496, 0.695132, 0.71807),
     vec3(0.0243487, 0.991858, 0.125),
     vec3(-3.86359e-009, 0.0883887, 0.996086),
     vec3(-0.0174883, 0.712395, 0.701561),
     vec3(-0.0249143, 0.507141, 0.861503),
     vec3(-0.0640398, 0.868166, 0.492125),
     vec3(-0.0357209, 0.36268, 0.931229),
     vec3(-0.0973771, 0.789513, 0.60596),
     vec3(-0.0907849, 0.612022, 0.785613),
     vec3(-0.160633, 0.925748, 0.342327),
     vec3(-0.0517311, 0.26007, 0.964203),
     vec3(-0.165463, 0.736841, 0.655506),
     vec3(-0.137517, 0.549, 0.824432),
     vec3(-0.241565, 0.872903, 0.423896),
     vec3(-0.128289, 0.422912, 0.897044),
     vec3(-0.261565, 0.791768, 0.551985),
     vec3(-0.224813, 0.628309, 0.744773),
     vec3(-0.349916, 0.907123, 0.233854),
     vec3(-0.0756345, 0.182598, 0.980274),
     vec3(-0.297532, 0.671221, 0.678924),
     vec3(-0.229873, 0.486026, 0.843171),
     vec3(-0.399386, 0.793444, 0.459279),
     vec3(-0.190938, 0.357219, 0.914296),
     vec3(-0.401663, 0.709034, 0.579601),
     vec3(-0.330813, 0.551929, 0.765466),
     vec3(-0.511493, 0.807736, 0.293151),
     vec3(-0.177054, 0.26498, 0.947859),
     vec3(-0.4466, 0.634123, 0.631219),
     vec3(-0.353206, 0.476243, 0.805256),
     vec3(-0.567737, 0.727487, 0.385276),
     vec3(-0.301962, 0.367942, 0.879453),
     vec3(-0.556756, 0.645434, 0.522913),
     vec3(-0.463601, 0.511504, 0.72349),
     vec3(-0.681412, 0.715709, 0.153093),
     vec3(-0.108253, 0.108253, 0.988212),
     vec3(-0.523985, 0.498876, 0.690335),
     vec3(-0.387453, 0.351167, 0.852386),
     vec3(-0.66593, 0.574435, 0.475986),
     vec3(-0.297822, 0.244416, 0.922801),
     vec3(-0.634821, 0.495419, 0.592927),
     vec3(-0.507, 0.376017, 0.775605),
     vec3(-0.774955, 0.545785, 0.318689),
     vec3(-0.243746, 0.162866, 0.956066),
     vec3(-0.646706, 0.409522, 0.643477),
     vec3(-0.49714, 0.297975, 0.8149),
     vec3(-0.795517, 0.450655, 0.405046),
     vec3(-0.405048, 0.216503, 0.888292),
     vec3(-0.753141, 0.379099, 0.537645),
     vec3(-0.61374, 0.290277, 0.734209),
     vec3(-0.896176, 0.397248, 0.197642),
     vec3(-0.216053, 0.0894919, 0.972272),
     vec3(-0.694868, 0.26804, 0.667317),
     vec3(-0.519718, 0.185958, 0.833854),
     vec3(-0.851768, 0.281386, 0.441942),
     vec3(-0.405643, 0.12305, 0.905711),
     vec3(-0.794567, 0.219886, 0.565962),
     vec3(-0.635861, 0.159275, 0.75519),
     vec3(-0.940775, 0.211258, 0.265165),
     vec3(-0.335749, 0.0667846, 0.939581),
     vec3(-0.774047, 0.13431, 0.618718),
     vec3(-0.599401, 0.0889127, 0.795495),
     vec3(-0.924226, 0.113992, 0.364434),
     vec3(-0.489756, 0.0482367, 0.870524),
     vec3(-0.859169, 0.063376, 0.507752),
     vec3(-0.700716, 0.0344238, 0.71261),
     vec3(-0.995786, 0.0244452, 0.0883883),
     vec3(-0.0624999, -5.46392e-009, 0.998045),
     vec3(-0.70965, -0.017421, 0.704339),
     vec3(-0.503284, -0.0247248, 0.863767),
     vec3(-0.865925, -0.0638745, 0.496078),
     vec3(-0.357306, -0.0351916, 0.933324),
     vec3(-0.787072, -0.0970762, 0.609175),
     vec3(-0.608891, -0.0903205, 0.788095),
     vec3(-0.923698, -0.160277, 0.347985),
     vec3(-0.252742, -0.0502736, 0.966227),
     vec3(-0.734313, -0.164896, 0.658478),
     vec3(-0.545643, -0.136676, 0.826797),
     vec3(-0.870822, -0.240989, 0.428478),
     vec3(-0.418661, -0.127, 0.899218),
     vec3(-0.789541, -0.260829, 0.555512),
     vec3(-0.625547, -0.223824, 0.747391),
     vec3(-0.905246, -0.349192, 0.242061),
     vec3(-0.173228, -0.0717532, 0.982265),
     vec3(-0.668784, -0.296452, 0.681795),
     vec3(-0.482731, -0.228315, 0.845484),
     vec3(-0.791477, -0.398396, 0.463512),
     vec3(-0.352941, -0.188651, 0.91643),
     vec3(-0.706946, -0.40048, 0.582961),
     vec3(-0.549319, -0.329249, 0.768013),
     vec3(-0.806008, -0.510399, 0.299739),
     vec3(-0.259834, -0.173616, 0.949918),
     vec3(-0.63206, -0.445147, 0.634306),
     vec3(-0.47359, -0.351239, 0.807678),
     vec3(-0.725817, -0.566433, 0.390312),
     vec3(-0.364756, -0.299348, 0.881671),
     vec3(-0.643697, -0.555257, 0.526634),
     vec3(-0.509404, -0.461697, 0.726184),
     vec3(-0.714277, -0.680048, 0.165359),
     vec3(-0.0988212, -0.0988212, 0.990186),
     vec3(-0.497011, -0.522027, 0.693159),
     vec3(-0.348649, -0.384675, 0.854675),
     vec3(-0.572982, -0.664246, 0.480072),
     vec3(-0.241179, -0.293877, 0.924916),
     vec3(-0.493925, -0.632906, 0.596212),
     vec3(-0.374169, -0.504508, 0.778119),
     vec3(-0.544597, -0.773269, 0.32476),
     vec3(-0.159121, -0.238142, 0.958107),
     vec3(-0.408155, -0.644547, 0.646505),
     vec3(-0.296237, -0.494242, 0.817294),
     vec3(-0.449601, -0.793656, 0.40984),
     vec3(-0.214489, -0.40128, 0.890488),
     vec3(-0.378056, -0.751069, 0.541266),
     vec3(-0.289045, -0.611134, 0.736864),
     vec3(-0.396439, -0.894353, 0.207289),
     vec3(-0.0862365, -0.208194, 0.974279),
     vec3(-0.267095, -0.692417, 0.670238),
     vec3(-0.184762, -0.516376, 0.836193),
     vec3(-0.280702, -0.849698, 0.446339),
     vec3(-0.121706, -0.401209, 0.907865),
     vec3(-0.219254, -0.792281, 0.569402),
     vec3(-0.158549, -0.632964, 0.757772),
     vec3(-0.210813, -0.938797, 0.272431),
     vec3(-0.0656621, -0.330106, 0.941657),
     vec3(-0.133884, -0.771593, 0.621867),
     vec3(-0.0884386, -0.596205, 0.797947),
     vec3(-0.113735, -0.922142, 0.369755),
     vec3(-0.047846, -0.48579, 0.872765),
     vec3(-0.0632089, -0.856905, 0.511585),
     vec3(-0.0342868, -0.69793, 0.715345),
     vec3(-0.0243971, -0.993824, 0.108253),
     vec3(1.29091e-009, -0.108253, 0.994123),
     vec3(0.0175555, -0.71513, 0.698771),
     vec3(0.0251023, -0.510968, 0.859233),
     vec3(0.0642048, -0.8704, 0.488141),
     vec3(0.0362424, -0.367974, 0.929129),
     vec3(0.0976775, -0.791946, 0.602728),
     vec3(0.0912468, -0.615136, 0.783123),
     vec3(0.160988, -0.927794, 0.336573),
     vec3(0.0531487, -0.267197, 0.962175),
     vec3(0.166029, -0.73936, 0.652519),
     vec3(0.138354, -0.552338, 0.822059),
     vec3(0.242139, -0.874978, 0.419263),
     vec3(0.129566, -0.42712, 0.894864),
     vec3(0.262298, -0.793989, 0.548435),
     vec3(0.225796, -0.631058, 0.742146),
     vec3(0.350638, -0.908995, 0.225347),
     vec3(0.0793261, -0.19151, 0.97828),
     vec3(0.298608, -0.673648, 0.676041),
     vec3(0.231421, -0.489298, 0.840851),
     vec3(0.400374, -0.795405, 0.455007),
     vec3(0.193197, -0.361447, 0.912157),
     vec3(0.402843, -0.711116, 0.576222),
     vec3(0.33237, -0.554526, 0.76291),
     vec3(0.512585, -0.80946, 0.286411),
     vec3(0.180427, -0.270028, 0.945797),
     vec3(0.448047, -0.636178, 0.628117),
     vec3(0.355163, -0.478882, 0.802827),
     vec3(0.569037, -0.729154, 0.380173),
     vec3(0.304554, -0.3711, 0.877229),
     vec3(0.55825, -0.647167, 0.519164),
     vec3(0.465497, -0.513597, 0.720785),
     vec3(0.682774, -0.717139, 0.139754),
     vec3(0.116927, -0.116927, 0.986233),
     vec3(0.525937, -0.500734, 0.6875),
     vec3(0.39021, -0.353666, 0.850092),
     vec3(0.667609, -0.575884, 0.471865),
     vec3(0.301716, -0.247611, 0.920682),
     vec3(0.63673, -0.496909, 0.589624),
     vec3(0.509479, -0.377855, 0.773082),
     vec3(0.776639, -0.54697, 0.3125),
     vec3(0.249224, -0.166526, 0.954021),
     vec3(0.648858, -0.410885, 0.640434),
     vec3(0.500023, -0.299702, 0.8125),
     vec3(0.797374, -0.451707, 0.400195),
     vec3(0.408781, -0.218498, 0.88609),
     vec3(0.755207, -0.380139, 0.534),
     vec3(0.616335, -0.291505, 0.731544),
     vec3(0.897996, -0.398054, 0.1875),
     vec3(0.223636, -0.0926329, 0.970261),
     vec3(0.697311, -0.268982, 0.664384),
     vec3(0.523039, -0.187146, 0.831508),
     vec3(0.853833, -0.282068, 0.4375),
     vec3(0.410028, -0.124381, 0.903552),
     vec3(0.796848, -0.220517, 0.5625),
     vec3(0.638745, -0.159997, 0.7526),
     vec3(0.942749, -0.211701, 0.257694),
     vec3(0.341299, -0.0678885, 0.9375),
     vec3(0.776492, -0.134734, 0.615554),
     vec3(0.602581, -0.0893843, 0.793036),
     vec3(0.926305, -0.114249, 0.359035),
     vec3(0.49369, -0.048624, 0.868278),
     vec3(0.861427, -0.0635427, 0.503891),
     vec3(0.703491, -0.0345603, 0.709864),
     vec3(0.997744 -0.0244931 0.0625)
);

const float PI = 3.14159265358979;

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

vec3 importanseSampleGGX(in vec2 xi,in float roughness,in vec3 norm)
{
	float a = roughness*roughness;
	float phi = 2.0*pi.xi.x;
	float cosTheta = sqrt((1.0-xi.y)/(1.0 + (a*a - 1.0)*xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	vec3 H;
	H.x = sinTheta*cos(phi);
	H.y = sinTheta*sin(phi);
	H.z = cosTheta;

	vec3 upVector = abs(norm.z)<0.999?vec3(0.0,0.0,1.0):vec3(1.0,0.0,0.0);
	vec3 tangentX = normalize(cross(upVector,norm));
	vec3 tangentY = cross(norm,tangentX);

	return tangentX*H.x + tangentY*H.y + norm*H.z;
}


/*diffuse lights models*/
float disneyDiffuse(in float FD90,in float NdotL, in float NdotV)
{
	return (1.0 + (FD90 - 1.0)*pow(1.0 - NdotL,5.0))*(1.0 + (FD90 - 1.0)*pow(1.0 - NdotV,5.0));
}

float GGXNDF(in float NdotH, in float roughness)
{
	float alpha = roughness*roughness;
	alpha = alpha*alpha;
	float NdotH2 = NdotH*NdotH;
	float div = NdotH2*(alpha - 1.0) + 1.0;
	return alpha/(pi*div*div);
}

float GGXGeometryShilick(in float roughness, in float NdotL, in float NdotV)
{
	float k = (roughness + 1.0)*(roughness + 1.0);
	float G1 = NdotV/(NdotV*(1.0 - k) + k);
	float G2 = NdotL/(NdotL*(1.0 - k) + k);
	return G1*G2;
}

float schlick(in float f0,in float fd90,in float VdotH)
{
	return (f0 + (fd90 - f0)*pow(1.0-VdotH,5.0);
}


/*from forbite2 engine*/
vec3 translucecnyAproximation(in float distorion, in float lightPower, in float lightScale, in vec3 lightDirection, in vec3 lightAmbient, in float thickness, in vec3 lightAuttenuation, in vec3 diffuseAlbedo, in vec3 lightDiffuse, in vec3 eyeDirection)
{
	vec3 transmitacneLightector = lightDirection + normal*distorion;
	float TLdotE = pow(clamp(dot(eyeDirection,-transmitacneLightector),lightPower),0.0,1.0)*lightScale;
	vec3 flt = lightAuttenuation*(TLdotE + lightAmbient)*thickness;
	return diffuseAlbedo*lightDiffuse*flt;
}

out vec4 outColor;

void main()
{
	/*variables for all lights*/
	mat3 TBN = mat3(tangent,bitangent,normal);
	vec3 eyeDirection = eyePosition-position;
	vec3 lightDirection = lightPosition - position;
	vec3 norm = normalize(normal);
	
	eyeDirection = TBN*eyeDirection;
	lightDirection = TBN*lightDirection;

	if(isLeaf)
	{
		float thickness = texture2D(leafThicknessMap,textureCoordinates);
		float opacity = texture2D(leafAlphaMask,textureCoordinates);
	
		vec3 frontDiffuseAlbedo;
		vec3 frontTranslucency;
		vec3 backDiffuseAlbedo;
		vec3 backTranslucency;
		vec3 normalLeaf;
		if(gl_FrontFacing == true)
		{
			frontDiffuseAlbedo = texture2D(leafFront,textureCoordinates);
			frontTranslucency = texture2D(leafFrontTranslucecy,textureCoordinates);
			backDiffuseAlbedo = texture2D(leafBack,textureCoordinates);
			backTranslucency = texture2D(leafBackTranslucecy,textureCoordinates);
			normalLeaf = (texture2D(leafFrontNormalMap,textureCoordinates)-0.5)*2.0;
		}
		else
		{
			backDiffuseAlbedo = texture2D(leafFront,textureCoordinates);
			backTranslucency = texture2D(leafFrontTranslucecy,textureCoordinates);
			frontDiffuseAlbedo = texture2D(leafBack,textureCoordinates);
			frontTranslucency = texture2D(leafBackTranslucecy,textureCoordinates);
			normalLeaf = (texture2D(leafBackNormalMap,textureCoordinates) - 0.5)*2.0;
			norm = -norm;
		}

		float NdotL;
		float NdotH;
		float VdotH;
		float NdotV = dot(normalLeaf,eyeDirection);
		/*light from the skyBox*/
		vec3 diff = vec3(0.0);
		vec3 spec = vec3(0.0);
		vec3 transm = vec3(0.0);
		for(int i=0;i<N;++i)
		{
			vec2 x = hamerslley(i,N);
			vec3 H = importanseSampleGGX(leafRougness,x,norm);
			vec3 L = 2*dot(eyeDirection,H)*H - eyeDirection;
			NdotL = dot(norm,L);
			NdotH = dot(norm,H);
			VdotH = dot(eyeDirection,H);

			vec3 sampleColor = texture(cubeMap,L).rgb;
			float G = GGXGeometryShilick(leafRougness,NdotL,NdotV);
			float Fc = pow(1.0 - VdotH,5.0);

			spec+=sampleColor*((1.0-Fc)*
			
		}
		
		/*light from the sun*/

		NdotL = dot(norm,lightDirection);
		if(NdotL>0)
		{
			vec3 half = normalize(lightDirection + eyeDirection);
			NdotH = dot(normalLeaf,half);
			VdotH = dot(half,eyeDirection);
			float ndf = GGXNDF(NdotH,leafRougness);
			float g = GGXGeometryShilick(roughness,NdotL,NdotV);
			float f = schlick(leafF0,1.0,VdotH);
			float fd90 = 0.5 * 2.0*VdotH*VdotH;
			float dissney = disneyDiffuse(fd90,NdotL,NdotV);

			outColor+= vec4(dissney*frontDiffuseAlbedo + ndf*g*f/(4.0*NdotL*NdotV),1.0);
			
		}
		else
		{
			normalLeaf.xy*=-1.0;
			vec3 t = translucecnyAproximation(1.1,4.0,2.5,lightDirection,vec3(0.1,0.1,.1),thickness,0.9,backDiffuseAlbedo,backTranslucency,eyeDirection);
			outColor+=vec4(t,1.0);
		}
		
	}
	else
	{
	}
			
}
