#pragma once
#include "Surface.h"
class Sphere : public Surface {
public:
	float radius;
	glm::vec3 center;
	
	Sphere(float r, glm::vec3 cent, Material& m);
	bool DetectHit(Ray& ray, float t_min, float t_max, Hit& hitRecord) override;
	void DisplayValuesInEditor(int index) override;

private:
	
};