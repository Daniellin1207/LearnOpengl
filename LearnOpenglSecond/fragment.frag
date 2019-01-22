#version 330 core			
in vec4 vertexColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;			
uniform sampler2D faceTexture;
//uniform vec4 ourColor;	
out vec4 FragColor;
void main()					
{							
	FragColor = mix(texture(ourTexture,TexCoord),texture(faceTexture,TexCoord),0.2);	
}									