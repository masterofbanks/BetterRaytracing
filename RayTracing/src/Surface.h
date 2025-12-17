#pragma once
#include "Hit.h"
#include "Material.h"
class Surface {
public:
	Material mat;

	virtual ~Surface() = default;
	virtual bool DetectHit(Ray& ray, float t_min, float t_max, Hit& hitRecord) = 0;
};