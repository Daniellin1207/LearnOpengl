#pragma once

#include <string>
#include <fstream>
#include <sstream>
#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

class Shader
{
public:
	unsigned int ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	string vertexString;
	string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	void use();
private:
	void checkCompileErrors(unsigned int ID, std::string type);
};

