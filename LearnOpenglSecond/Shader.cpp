#include "pch.h"
#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	ifstream vertexFile;
	ifstream fragmentFile;
	
	stringstream vertexSStream;
	stringstream fragmentSStream;
	
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw exception("open file error!");
		}
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		printf(vertexSource);
		printf(fragmentSource);
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}
