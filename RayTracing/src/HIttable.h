#pragma once
#include "Ray.h"
#include <glm/glm.hpp>

class Hit {
public:
	glm::vec3 p;
	glm::vec3 normal;
	float t;
};