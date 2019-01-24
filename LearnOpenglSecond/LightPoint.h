#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


class LightPoint
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 angles;
	glm::vec3 color;

	float constant;
	float linear;
	float quadratic;
	LightPoint(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0, 1.0, 1.0));
	~LightPoint();

	void UpdateDirection() {
		direction = glm::vec3(0, 0, 1);
		direction = glm::rotateZ(direction, angles.z);
		direction = glm::rotateX(direction, angles.x);
		direction = glm::rotateY(direction, angles.y);
		direction = -direction;

	}
};
