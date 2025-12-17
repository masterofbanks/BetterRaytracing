#pragma once
#include <glm/glm.hpp>

class Material {
public:
	float k_diffuse;
	float k_phong;
	glm::vec3 albedo;

	Material();
	Material(float k_d, float k_p, glm::vec3& a);
};