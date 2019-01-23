#version 330 core			
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main()					
{				
	vec3 lightDir=normalize(lightPos-FragPos);
	vec3 diffuse=max(dot(lightDir,Normal),0)*lightColor*objColor;
//	FragColor = mix(texture(ourTexture,TexCoord),texture(faceTexture,TexCoord),0.2);	
	FragColor=vec4(diffuse,1.0);
}									