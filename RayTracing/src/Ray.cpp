#include "Ray.h"

Ray::Ray() {
	origin = glm::vec3(0, 0, 0);
	direction = glm::vec3(0, 0, 1);
}

Ray::Ray(glm::vec3 o, glm::vec3 d)
{
	origin = o;
	direction = d;
}

/// <summary>
/// Finds the x and y components of a ray at a parameter t
/// </summary>
/// <param name="t"></param>
/// <returns>the x and y components of a ray at a parameter t</returns>
const glm::vec3 Ray::CalculatePoint(float t)
{
	return origin + direction * t;
}
