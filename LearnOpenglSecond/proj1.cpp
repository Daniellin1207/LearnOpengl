#pragma region include
#include "pch.h"

#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

#include "Mesh.h"
#pragma endregion

#pragma region Camera Declare
Camera camera(glm::vec3(0, 0.0, 3.0f),10.0f,0.0f, glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Light Declare
LightDirectional lightD(
	glm::vec3(1,1,-1),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
	glm::vec3(1.0f, 1.0f, 1.0f)
);

LightPoint lightP1(
	glm::vec3(30,0,0),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
	glm::vec3(1.0f, 0,0)
);
LightPoint lightP2(
	glm::vec3(0,30,0),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
	glm::vec3(0, 1.0f,0)
);
LightPoint lightP3(
	glm::vec3(0,0,30),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
	glm::vec3(0,0, 1.0f)
);
LightPoint lightP4(
	glm::vec3(-30,0,-30),
	glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
	glm::vec3(1.0f, 1.0f, 1.0f)
);
LightSpot lightS(
	glm::vec3(0, 3, 0),
	glm::vec3(glm::radians(90.0f), 0, 0),
	glm::vec3(1.0f, 1.0f, 1.0f)
);
#pragma endregion

#pragma region Funcion Declare
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned LoadImageToGPU(const char* filename,GLint internalformat,GLenum format,int slot);
bool firstMouse = true;
double lastX, lastY;
#pragma endregion

#pragma region Data Declare
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float lightCube[] = {
	0,0.5,  2.0,
	0,0,    2.0,
	0.5,0,  2.0,

	0,0.5,  2.0,
	0.5,0,  2.0,
	0.5,0.5,2.0};
#pragma endregion

int main()
{
#pragma region Init Setting
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
#pragma endregion

#pragma region Shader
	Shader* testShader = new Shader("vertex.vert", "fragment.frag");
	Shader* myShader = new Shader("VertLight.vert", "FragLight.frag");
#pragma endregion

#pragma region Material
	Material* myMaterial = new Material(testShader,
		LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, 0),
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, 1),
		glm::vec3(0.1, 0.1, 0.1),
		32.0f);
#pragma endregion

#pragma region VAO and VBO

	Mesh cube(vertices);


	//unsigned int VAOs[2], VBOs[2], EBOs;

	//glGenVertexArrays(2, VAOs); // GenBuffers
	//glGenBuffers(2, VBOs);
	////glGenBuffers(1, &EBO);

	//glBindVertexArray(VAOs[0]);  // BindBuffers
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // BindDatas
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//glBindVertexArray(VAOs[1]);
	//// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	//// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lightCube), lightCube, GL_STATIC_DRAW); // BindDatas
	////glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // BindDatas
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

#pragma endregion

#pragma region Texture Setting
	//unsigned int TexBufferA, TexBufferB;
	//TexBufferA = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);
	//TexBufferB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

#pragma region GL_FUNCTION SETTING
		glClearColor(0,0,0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#pragma endregion

#pragma region Activate Texture in Circle
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, myMaterial->specular);
#pragma endregion

#pragma region TenCube Drawing
		testShader->use();
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 modelMat = glm::mat4(1.0f);
		glm::mat4 projMat = glm::mat4(1.0f); 
		for (int i = 0; i < 1; i++)
		{
			float angle = 20.0f*i+10.0;
			modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, cubePositions[i]);
			modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			viewMat = camera.GetViewMatrix();
			projMat = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 1000.0f);
			// 传Texture
			glUniform1i(glGetUniformLocation(testShader->ID, "ourTexture"), 0);
			glUniform1i(glGetUniformLocation(testShader->ID, "faceTexture"), 1);
			// 传Matrix
			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			// 传Light
			glUniform3f(glGetUniformLocation(testShader->ID, "objColor"),1,1,1);
			glUniform3f(glGetUniformLocation(testShader->ID, "ambientColor"),0.5,0.5,0.5);


			glUniform3f(glGetUniformLocation(testShader->ID, "lightD.pos"), lightD.position.x,lightD.position.y,lightD.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);

			glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.pos"), lightP1.position.x, lightP1.position.y, lightP1.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.color"), lightP1.color.x, lightP1.color.y, lightP1.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP1.dirToLight"), lightP1.direction.x, lightP1.direction.y, lightP1.direction.z);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.constant"), lightP1.constant);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.linear"), lightP1.linear);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP1.quadratic"), lightP1.quadratic);

			glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.pos"), lightP2.position.x, lightP2.position.y, lightP2.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.color"), lightP2.color.x, lightP2.color.y, lightP2.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP2.dirToLight"), lightP2.direction.x, lightP2.direction.y, lightP2.direction.z);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.constant"), lightP2.constant);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.linear"), lightP2.linear);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP2.quadratic"), lightP2.quadratic);

			glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.pos"), lightP3.position.x, lightP3.position.y, lightP3.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.color"), lightP3.color.x, lightP3.color.y, lightP3.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP3.dirToLight"), lightP3.direction.x, lightP3.direction.y, lightP3.direction.z);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.constant"), lightP3.constant);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.linear"), lightP3.linear);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP3.quadratic"), lightP3.quadratic);

			glUniform3f(glGetUniformLocation(testShader->ID, "lightP4.pos"), lightP4.position.x, lightP4.position.y, lightP4.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP4.color"), lightP4.color.x, lightP4.color.y, lightP4.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightP4.dirToLight"), lightP4.direction.x, lightP4.direction.y, lightP4.direction.z);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP4.constant"), lightP4.constant);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP4.linear"), lightP4.linear);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightP4.quadratic"), lightP4.quadratic);

			glUniform3f(glGetUniformLocation(testShader->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);
			glUniform3f(glGetUniformLocation(testShader->ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightS.cosPhyInner"), lightS.cosPhyInner);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightS.cosPhyOutter"), lightS.cosPhyOutter);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightS.constant"), lightS.constant);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightS.linear"), lightS.linear);
			glUniform1f(glGetUniformLocation(testShader->ID, "lightS.quadratic"), lightS.quadratic);

			glUniform3f(glGetUniformLocation(testShader->ID, "cameraPos"),camera.Position.x,camera.Position.y,camera.Position.z);

			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			myMaterial->shader->SetUniform1i("material.specular",Shader::SPECULAR);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);


			//glBindVertexArray(VAOs[0]);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			cube.draw(myMaterial->shader);
		}
#pragma endregion

#pragma region Operation myShader 
		//myShader->use();
		//glBindVertexArray(VAOs[1]);
		//glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		//glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

#pragma region Function Define
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Position += camera.Forward*camera.MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Position -= camera.Forward*camera.MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Position -= camera.Right*camera.MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Position += camera.Right*camera.MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.Position -= camera.Up*camera.MovementSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.Position += camera.Up*camera.MovementSpeed;
}

unsigned int LoadImageToGPU(const char* filename, GLint internalformat, GLenum format, int slot)
{
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);
	int width, height, nrChannel;
	stbi_set_flip_vertically_on_load(true); // 颠倒载入
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("load image failed.");
	}
	stbi_image_free(data);
	return TexBuffer;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	printf("mouseMove: %f %f\n", xpos, ypos);
	if (firstMouse == true) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}
	float deltaX, deltaY;
	deltaX = xpos - lastX;
	deltaY = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	printf("mouseDelta: %f %f\n", deltaX, deltaY);
	camera.ProcessMouseMovement(deltaX, -deltaY);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	printf("mouseScroll: %f %f\n", xoffset, yoffset);
	camera.ProcessMouseScroll(yoffset);
}
#pragma endregion