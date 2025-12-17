#pragma once
#include "Ray.h"
#include "Material.h"
#include <glm/glm.hpp>

class Hit {
public:
	glm::vec3 p;
	glm::vec3 normal;
	Material material;
	float t;
};
