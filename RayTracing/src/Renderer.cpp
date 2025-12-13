#include "Renderer.h"
#include "Walnut/Random.h"
#include <iostream>

Renderer::Renderer()
{
	//Sphere* redSphere = new Sphere(0.5f, glm::vec3(-1,0,3), glm::vec3(1,1,1));
	Sphere* blueSphere = new Sphere(0.5f, glm::vec3(0,0,3), glm::vec3(0, 0, 0.7f));
	Sphere* otherBlueSphere = new Sphere(2.0f, glm::vec3(0,0,10), glm::vec3(0, 0.7f, 0.7f));
	//scene.push_back(redSphere);
	scene.push_back(blueSphere);
	scene.push_back(otherBlueSphere);
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
void Renderer::Render() {
	
	float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	//loop through ys on the outside to be more friendly to the cpu cache
	//cpu can seach through the horizontally placed pixels faster
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) 
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // -1 -> 1
			coord.x *= aspectRatio;
			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = ConvertColorVectorToInt(color);

		}
	}
	
	

	m_FinalImage->SetData(m_ImageData);

}


/// <summary>
/// For a given camera coordinate, see if a ray shot from the camera's origin through a pixel as hit a sphere located at 0,0,0
/// </summary>
/// <param name="coord"></param>
/// <returns>the color of the collision; red it there was a collision, black if there was not</returns>
glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	//shoot a ray from the camera origin through one of the pixel "windows" of the 
	glm::vec3 origin = glm::vec3(0, 0, 0);
	glm::vec3 direction = glm::vec3(coord.x, coord.y, -1);
	Ray ray = Ray(origin, direction);

	//light values
	float k_diffuse = 0.95f;
	float k_ambient = 0.4f;
	
	glm::vec3 lightOrigin = glm::vec3(3, -3, 10);

	
	Hit tempHitRecord;
	float minimumCheckDistance = 0.001f;
	float maxCheckDistance = 999999999.0f;
	if (HitScene(ray, minimumCheckDistance, maxCheckDistance, tempHitRecord)) {
		glm::vec3 vectorToLight = glm::normalize(lightOrigin - tempHitRecord.p);
		float fallOffFactor = std::max(0.0f, glm::dot(tempHitRecord.normal, vectorToLight));
		glm::vec3 outputColor = k_ambient * tempHitRecord.colorOfHit + (k_diffuse * fallOffFactor) * tempHitRecord.colorOfHit;
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




