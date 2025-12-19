#include "Light.h"

Light::Light()
{
	albedo = glm::vec3(1, 1, 1);
	position = glm::vec3(0, 0, 0);

}

Light::Light(glm::vec3& a, glm::vec3& pos)
{
	albedo = a;
	position = pos;
}

const glm::vec3 Light::GetAlbedo()
{
	return albedo;
}

const glm::vec3 Light::GetOrigin()
{
	return position;
}
