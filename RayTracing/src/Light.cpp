#include "Light.h"
#include "Walnut/Application.h"
#include <glm/gtc/type_ptr.hpp>
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

void Light::DisplayValuesInEditor(int index)
{
	ImGui::PushID(index);
	ImGui::DragFloat3("Light Position", glm::value_ptr(position), 0.1f);
	ImGui::ColorEdit3("Light Albedo", glm::value_ptr(albedo), 0.1f);
	ImGui::Separator();
	ImGui::PopID();
}

