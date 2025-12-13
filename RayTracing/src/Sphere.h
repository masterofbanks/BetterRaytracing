#pragma once
#include "Surface.h"
class Sphere : public Surface {
public:
	Sphere(float r, glm::vec3 cent, glm::vec3 col);
	bool DetectHit(Ray& ray, float t_min, float t_max, Hit& hitRecord) override;

private:
	float radius;
	glm::vec3 center;
	glm::vec3 color;
};