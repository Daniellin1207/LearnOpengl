#include "pch.h"
#include "LightSpot.h"

LightSpot::LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color) :
	position(_position), angles(_angles), color(_color)
{
	constant = 1;
	linear = 0.9;
	quadratic = 0.09;
	UpdateDirection();
}