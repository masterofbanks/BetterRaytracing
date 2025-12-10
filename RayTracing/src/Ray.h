#pragma once
#include <glm/glm.hpp>

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(glm::vec3 o, glm::vec3 d);
	glm::vec3 CalculatePoint(float t);
};