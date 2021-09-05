#version 330



struct Material
{
	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 SpecularColour;
	float specularIntensity;
	bool isShiny;
	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	sampler2D diffuseTexture;
	sampler2D SpecularTexture;
};

uniform Material materialObject;
uniform vec3 ambientLight;

in vec2 texCoord;
in vec3 fragmentPosition;
in vec3 fragmentNormal;

out vec4 fragment_colour;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 ambientScene;
vec3 surfaceDiffuse;
vec3 surfaceSpecular;

void main(void)
{

	float specularStrength = 1;
	float ambientStrength = 0.2;

	if (materialObject.hasDiffuseTexture)
	{
		surfaceDiffuse = (texture(materialObject.diffuseTexture, texCoord) * vec4(materialObject.diffuseColour, 1.0)).xyz;
	}
	else
	{
		surfaceDiffuse = materialObject.diffuseColour;
	}

	if (materialObject.hasSpecularTexture)
	{
		surfaceSpecular = (texture(materialObject.SpecularTexture, texCoord) * vec4(materialObject.SpecularColour, 1.0)).xyz;
	}
	else
	{
		surfaceSpecular = materialObject.SpecularColour;
	}

	vec3 ambient = ambientStrength * materialObject.ambientColour;
	vec3 normal = normalize(fragmentNormal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuse = max(dot(normal,lightDirection),0.0);



	vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDirection, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),materialObject.specularIntensity);
	vec3 specular = surfaceSpecular * spec;  

	vec3 finalDiffuse =  diffuse * surfaceDiffuse;
	vec3 result;
	if (materialObject.isShiny)
	{
		result = ambient + ( finalDiffuse + specular);

	}

	else
	{
		result = ambient + finalDiffuse ;
	}


	fragment_colour = vec4(result, 1.0);
	
	
	
    
}
