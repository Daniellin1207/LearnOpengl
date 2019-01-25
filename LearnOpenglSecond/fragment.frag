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

struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};

struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;

	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;

	float constant;
	float linear;
	float quadratic;

	float cosPhyInner;
	float cosPhyOutter;
};

uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP1,lightP2,lightP3,lightP4;
uniform LightSpot lightS;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 lightDirUniform;
uniform vec3 lightPos;
uniform vec3 cameraPos;

out vec4 FragColor;

vec3 CalcLightDirectional(LightDirectional light,vec3 uNormal,vec3 dirToCamera){
	vec3 result=vec3(0,0,0);
	// diffuse
	float diffIntensity=max(dot(light.dirToLight,uNormal),0);
	vec3 diffuseColor=diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb;
	// specular
	vec3 R=normalize(reflect(-light.dirToLight,Normal));
	float specIntensity=pow(max(dot(R,dirToCamera),0),material.shininess);\
	vec3 specColor=specIntensity*light.color*texture(material.specular,TexCoord).rgb;

	result+=diffuseColor+specColor;
	return result;
}
vec3 CalcLightPoint(LightPoint light,vec3 uNormal,vec3 dirToCamera){
	vec3 result=vec3(0,0,0);
	float dist=length(FragPos-light.pos);
	float attenuation=1/(light.constant+light.linear*dist+light.quadratic*dist*dist);

	// diffuse
	float diffIntensity=max(dot(light.dirToLight,uNormal),0);
	vec3 diffuseColor=diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb;
	// specular
	vec3 R=normalize(reflect(-light.dirToLight,Normal));
	float specIntensity=pow(max(dot(R,dirToCamera),0),material.shininess);
	vec3 specColor=specIntensity*light.color*texture(material.specular,TexCoord).rgb;

	result+=diffuseColor+specColor;
	return result*attenuation*100;
}

vec3 CalcLightSpot(LightSpot light,vec3 uNormal,vec3 dirToCamera){
	vec3 result=vec3(0,0,0);
	float dist=length(FragPos-light.pos);
	float attenuation=1/(light.constant+light.linear*dist+light.quadratic*dist*dist);

	// diffuse
	float diffIntensity=max(dot(light.dirToLight,uNormal),0);
	vec3 diffuseColor=diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb;
	// specular
	vec3 R=normalize(reflect(-light.dirToLight,Normal));
	float specIntensity=pow(max(dot(R,dirToCamera),0),material.shininess);
	vec3 specColor=specIntensity*light.color*texture(material.specular,TexCoord).rgb;
	
	float spotRatio;
	float cosTheta=dot(normalize(FragPos-light.pos),-light.dirToLight);
	if(cosTheta>light.cosPhyInner){
		// inside
		spotRatio=1.0;
	}else if(cosTheta>light.cosPhyOutter){
		// outside
		spotRatio=(cosTheta-light.cosPhyOutter)/(light.cosPhyInner-light.cosPhyOutter);
	}else{
		spotRatio=0;
	};
	result+=(diffuseColor+specColor)*spotRatio*attenuation;
	return result;
}

void main()					
{
	vec3 finalResult=vec3(0,0,0);
	vec3 uNormal=normalize(Normal);
	vec3 dirToCamera=normalize(cameraPos-FragPos);
	finalResult+=CalcLightDirectional(lightD,uNormal,dirToCamera);
	finalResult+=CalcLightPoint(lightP1,uNormal,dirToCamera);
	finalResult+=CalcLightPoint(lightP2,uNormal,dirToCamera);
	finalResult+=CalcLightPoint(lightP3,uNormal,dirToCamera);
	finalResult+=CalcLightPoint(lightP4,uNormal,dirToCamera);
	finalResult+=CalcLightSpot(lightS,uNormal,dirToCamera);

	FragColor=vec4(finalResult,1);
}									