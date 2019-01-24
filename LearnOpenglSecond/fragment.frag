#version 330 core			
in vec3 FragPos;
in vec3 Normal;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()					
{				
	vec3 lightDir=normalize(lightPos-FragPos);
	vec3 reflectVec=reflect(-lightDir,Normal);
	vec3 cameraVec=normalize(cameraPos-FragPos);

	// diffuse
	vec3 diffuse=max(dot(lightDir,Normal),0)*lightColor;
	// specular
	float specularAmount=pow(max(dot(reflectVec,cameraVec),0),material.shininess);
	vec3 specular=specularAmount*lightColor;
	// ambient
	vec3 ambient=material.ambient*ambientColor;
	FragColor=vec4((ambient+diffuse+specular)*objColor,1.0);
}									