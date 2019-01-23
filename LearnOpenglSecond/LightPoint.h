#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class LightPoint
{
public:
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Color;
	LightPoint(glm::vec3 position, glm::vec3 forward, glm::vec3 color) :Position(position), Forward(forward), Color(color) {};
	~LightPoint();
};

