#pragma once
#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 albedo;
	glm::vec3 position;
public:
	Light();
	Light(glm::vec3& a, glm::vec3& pos);
	const glm::vec3 GetAlbedo();
	const glm::vec3 GetOrigin();
};