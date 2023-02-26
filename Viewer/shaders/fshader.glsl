#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


// We set this field's properties from the C++ code
uniform Material material;
uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 specularLight;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform int intensity;


// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;

// The final color of the fragment (pixel)
out vec4 frag_color;



void main()
{
// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));


	vec3 ambient = (ambientLight * material.ambient);


	vec3 lightDir = normalize ( lightPosition - fragPos);
	vec3 normal = normalize(fragNormal);
	float diff = max(dot(normal,lightDir),0.0f);
	vec3 diffuse = diff * (diffuseLight * material.diffuse);

	vec3 viewDir = normalize ( cameraPosition - fragPos);
	vec3 reflect =normalize(reflect(-lightDir, normal));
	float spec = pow(max((dot(reflect, viewDir)),0.0f), intensity);
	vec3 specular = specularLight * spec * material.specular;


	if (material.ambient == vec3(0,0,0))
	{
		ambient = vec3(0, 0, 0);
	}
	if (material.diffuse == vec3(0, 0, 0))
	{
		diffuse = vec3(0, 0, 0);
	}
	if (material.specular == vec3(0, 0, 0))
	{
		specular = vec3(0, 0, 0);
	}

	vec3 theOutput = ambient + diffuse + specular;
	if (theOutput.x > 1) { theOutput.x = 1; }
	if (theOutput.x < 0) { theOutput.x = 0; }
	if (theOutput.y > 1) { theOutput.y = 1; }
	if (theOutput.y < 0) { theOutput.y = 0; }
	if (theOutput.z > 1) { theOutput.z = 1; }
	if (theOutput.z < 0) { theOutput.z = 0; }


	frag_color = vec4(theOutput,1);


	
}

