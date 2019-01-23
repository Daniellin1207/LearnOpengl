#version 330 core			
in vec3 FragPos;
in vec3 Normal;

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

	vec3 diffuse=max(dot(lightDir,Normal),0)*lightColor;
	float specularAmount=pow(max(dot(reflectVec,cameraVec),0),128);
	vec3 specular=specularAmount*lightColor;
//	FragColor = mix(texture(ourTexture,TexCoord),texture(faceTexture,TexCoord),0.2);	
	FragColor=vec4((diffuse+specular)*objColor,1.0);
}									