#version 330 core			
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightPoint{
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	float cosPhyInner;
	float cosPhyOutter;
};

uniform LightSpot lightS;
uniform LightPoint lightP;
uniform Material material;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 lightDirUniform;
uniform vec3 lightPos;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()					
{				
//	float dist=length(lightPos-FragPos);
//	float attenuation=1/(lightP.constant+lightP.linear*dist+lightP.quadratic*dist*dist);

	vec3 lightDir=normalize(lightPos-FragPos);
	vec3 reflectVec=reflect(-lightDir,Normal);
	vec3 cameraVec=normalize(cameraPos-FragPos);

	// specular
	float specularAmount=pow(max(dot(reflectVec,cameraVec),0),material.shininess);
	vec3 specular=texture(material.specular,TexCoord).rgb*specularAmount*lightColor;
	// diffuse
	float diffuseAmount=max(dot(lightDir,Normal),0);
	vec3 diffuse=texture(material.diffuse,TexCoord).rgb*diffuseAmount*lightColor;
	// ambient
	vec3 ambient=material.ambient*ambientColor*texture(material.diffuse,TexCoord).rgb;

	float cosTheta=dot(normalize(FragPos-lightPos),lightDirUniform);
	float spotRatio;
	if(cosTheta>lightS.cosPhyInner){
		// inside
		spotRatio=1.0;
		FragColor=vec4((ambient+diffuse+specular)*objColor,1.0);
	}else if(cosTheta>lightS.cosPhyOutter){
		// outside
		spotRatio=(cosTheta-lightS.cosPhyOutter)/(lightS.cosPhyInner-lightS.cosPhyOutter);
	}else{
		spotRatio=0;
	};
	FragColor=vec4((ambient+(diffuse+specular)*spotRatio)*objColor,1.0);
}									