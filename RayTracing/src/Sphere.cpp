#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
Sphere::Sphere(float r, glm::vec3 cent, Material& m)
{
	radius = r;
	center = cent;
	mat = m;
}

/// <summary>
/// Shoot a ray into the scene and find the closer intersection between the ray and a sphere
/// </summary>
/// <param name="ray"></param>
/// <param name="radius"></param>
/// <param name="sphereCenter"></param>
/// <param name="hitRecord"></param>
/// <returns>Whether or not the ray hit the sphere, and stores the properties of the hit inside a hit record</returns>
bool Sphere::DetectHit(Ray& ray, float t_min, float t_max, Hit& hitRecord)
{
	float a = glm::dot(ray.direction, ray.direction);
	float b = -2 * glm::dot(ray.direction, center - ray.origin);
	float c = glm::dot((center - ray.origin), (center - ray.origin)) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
	}

	//only record the closer collision for now
	float testT = (-b - std::sqrt(discriminant)) / (2.0f * a);
	if (testT > t_max) {
		return false;
	}
	hitRecord.t = testT;
	hitRecord.p = ray.CalculatePoint(hitRecord.t);
	hitRecord.normal = glm::normalize(hitRecord.p - center);
	hitRecord.material = mat;
	return true;
}

void Sphere::DisplayValuesInEditor(int index)
{
	ImGui::Begin("Scene");
	ImGui::PushID(index);
	ImGui::DragFloat3("Postition", glm::value_ptr(center), 0.1f);
	ImGui::DragFloat("Radius", &radius, 0.1f, 0.001f);
	mat.DisplayValues();
	ImGui::PopID();
	ImGui::Separator();
	ImGui::End();

}

