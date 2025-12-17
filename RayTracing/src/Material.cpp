#include "Material.h"

Material::Material()
{
	k_diffuse = 0.5f;
	k_phong = 0.5f;
	albedo = glm::vec3(1, 0, 0);
}

Material::Material(float k_d, float k_p, glm::vec3& a)
{
	k_diffuse = k_d;
	k_phong = k_p;
	albedo = a;
}
