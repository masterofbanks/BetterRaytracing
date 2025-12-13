#pragma once
#include "Hit.h"
class Surface {
public:
	virtual ~Surface() = default;
	virtual bool DetectHit(Ray& ray, float t_min, float t_max, Hit& hitRecord) = 0;
};