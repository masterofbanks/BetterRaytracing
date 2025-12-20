#include "Material.h"
#include "Walnut/Application.h"
#include <glm/gtc/type_ptr.hpp>
Material::Material()
{
	k_diffuse = 0.5f;
	k_phong = 0.5f;
	albedo = glm::vec3(1, 1, 1);
}

Material::Material(float k_d, float k_p, glm::vec3& a)
{
	k_diffuse = k_d;
	k_phong = k_p;
	albedo = a;
}

void Material::DisplayValues()
{
	ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo));
	ImGui::DragFloat("Diffuse Lighting Scaling", &k_diffuse, 0.01f, 0, 1.0f);
	ImGui::DragFloat("Phong Lighting Scaling", &k_phong, 0.01f, 0, 1.0f);
}
