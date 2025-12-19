#include "Renderer.h"
#include "Walnut/Random.h"
#include <iostream>
#include <cmath>

Renderer::Renderer()
{
	//Sphere* redSphere = new Sphere(0.5f, glm::vec3(-1,0,3), glm::vec3(1,1,1));
	Material blueMaterial = Material(0.95f, 0.95f, glm::vec3(0, 0, 0.7f));
	Sphere* blueSphere = new Sphere(0.5f, glm::vec3(1,0,-3), blueMaterial);

	Material redMaterial = Material(0.95f, 0.4f, glm::vec3(0.7f, 0, 0));
	Sphere* redSphere = new Sphere(3.0f, glm::vec3(0,0,-10), redMaterial);
	//Sphere* bigSphere = new Sphere(100, glm::vec3(0, -105, -25), glm::vec3(0, 0.4f, 0));

	scene.push_back(redSphere);
	scene.push_back(blueSphere);
	//scene.push_back(bigSphere);



	glm::vec3 colorOfLight = glm::vec3(1, 1, 1);
	glm::vec3 lightOrigin = glm::vec3(10000, 10000, 10000);
	Light* light = new Light(colorOfLight, lightOrigin);

	glm::vec3 secondLightColor = glm::vec3(1, 1, 1);
	glm::vec3 secondLightOrigin = glm::vec3(-10000, 10000, 10000);
	Light* secondLight = new Light(secondLightColor, secondLightOrigin);

	lights.push_back(light);
	lights.push_back(secondLight);



}

/// <summary>
/// This method should get called whenever an image needs to be resized. Ex: viewport for our camera gets reized in the GUI
/// </summary>
/// <param name="width">New width of the image</param>
/// <param name="height">New height of the image</param>
void Renderer::OnResize(uint32_t width, uint32_t height)
{
	//if we already have an image, but the size of the actual window differes from the size image, resize the image to the new dimensions
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) {
			return;
		}
		m_FinalImage->Resize(width, height);
	}

	//else make a new image
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	//fill the cpu buffer with pixel data
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

}

/// <summary>
/// Render every pixel that makes up the viewport of the image
/// </summary>
void Renderer::Render(const Camera& camera) {

	Ray ray;
	ray.origin = camera.GetPosition();

	float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	//loop through ys on the outside to be more friendly to the cpu cache
	//cpu can seach through the horizontally placed pixels faster
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) 
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
		{
			ray.direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = ConvertColorVectorToInt(color);

		}
	}
	
	

	m_FinalImage->SetData(m_ImageData);

}


/// <summary>
/// For a given camera coordinate, see if a ray shot from the camera's origin through a pixel has hit the scene
/// </summary>
/// <param name="coord"></param>
/// <returns>the color of the collision; black if there was not any collision</returns>
glm::vec4 Renderer::TraceRay(Ray& ray)
{
	
	//light values
	float k_ambient = 0.0f;
	int phongExponent = 256;
	
	Hit tempHitRecord;
	float minimumCheckDistance = 0.001f;
	float maxCheckDistance = 999999999.0f;
	if (HitScene(ray, minimumCheckDistance, maxCheckDistance, tempHitRecord)) {
		glm::vec3 outputColor = glm::vec3();
		for (int i = 0; i < lights.size(); i++) {
			glm::vec3 vectorToLight = glm::normalize(lights[i]->GetOrigin() - tempHitRecord.p);

			//diffuse shading factor = k_f * n • l
			float diffuseFactor = tempHitRecord.material.k_diffuse * std::max(0.0f, glm::dot(tempHitRecord.normal, vectorToLight));

			//e = vector to camera from hit point
			//h = half vector = (e + l)/||e + l||
			glm::vec3 e = ray.origin - tempHitRecord.p;
			glm::vec3 h = glm::normalize(e + vectorToLight);

			//r = -l + 2(l • n)n
			glm::vec3 r = -vectorToLight + 2 * glm::dot(vectorToLight, tempHitRecord.normal) * tempHitRecord.normal;
			r = glm::normalize(r);

			//phong shading factor = k_p * (h • n)^p
			float phongShading = tempHitRecord.material.k_phong * std::pow(glm::dot(h, tempHitRecord.normal), phongExponent);

			//other phong shading factor = k_p * max(0,e • r)^p
			//float phongShading = k_phong * std::pow(std::max(0.0f, glm::dot(e, r)), phongExponent);

			outputColor += (k_ambient + diffuseFactor) * tempHitRecord.material.albedo + phongShading * lights[i]->GetAlbedo();
		}
		
		return glm::vec4(outputColor, 1);

	}

	return glm::vec4(0, 0, 0, 1);

	
}

/// <summary>
/// Detects whether a ray has hit any object in the scene, and stores the closest result in a hit record (t value, point, and normal)
/// </summary>
/// <param name="ray"></param>
/// <param name="ray_tmin"></param>
/// <param name="ray_tmax"></param>
/// <param name="hitRecord"></param>
/// <returns>Whether or not the ray hit the scene, and stores the hit information in a Hit structure</returns>
bool Renderer::HitScene(Ray& ray, float ray_tmin, float ray_tmax, Hit& hitRecord)
{
	Hit tempRecord;
	bool hitAnything = false;
	float closestSoFar = ray_tmax;
	for (int i = 0; i < scene.size(); i++) {
		if (scene[i]->DetectHit(ray, ray_tmin, closestSoFar, tempRecord)) {
			hitAnything = true;
			closestSoFar = tempRecord.t;
			hitRecord = tempRecord;

		}
	}

	return hitAnything;
}

/// <summary>
/// Converts a color vector stored with rgb values between 0 and 1 to a 32 bit integer (RGBA format)
/// </summary>
/// <param name="color"></param>
/// <returns>the 32 bit form of the color</returns>
uint32_t Renderer::ConvertColorVectorToInt(glm::vec4& color)
{
	uint8_t r = uint8_t(color.r * 255.0f);
	uint8_t g = uint8_t(color.g * 255.0f);
	uint8_t b = uint8_t(color.b * 255.0f);

	uint8_t a = uint8_t(color.a * 255.0f); 

	return
		(uint32_t(a) << 24) |
		(uint32_t(b) << 16) |
		(uint32_t(g) << 8) |
		(uint32_t(r));

}




