#pragma once
#include "Walnut/Image.h"
#include "Ray.h"
#include "Sphere.h"
#include <memory>
#include <glm/glm.hpp>
#include "Hit.h"
#include <vector>
#include "Camera.h"
#include "Light.h"

/// <summary>
/// Class that takes a scene description as an input, and produces the pixels of an image as an output.
/// </summary>
class Renderer {
public:
	std::vector<Surface*> scene;
	std::vector<Light*> lights;
	
	
	Renderer();

	/// <summary>
	/// This method should get called whenever an image needs to be resized. Ex: viewport for our camera gets reized in the GUI
	/// </summary>
	/// <param name="width">New width of the image</param>
	/// <param name="height">New height of the image</param>
	void OnResize(uint32_t width, uint32_t height);



	/// <summary>
	/// Render every pixel that makes up the viewport of the image
	/// </summary>
	void Render(const Camera& camera);

	/// <summary>
	/// Getter method for the final image of the render
	/// </summary>
	/// <returns>the final image</returns>
	std::shared_ptr<Walnut::Image> GetFinalImage() {
		return m_FinalImage;
	}

	void AddSphereToScene() {
		scene.push_back(new Sphere());
	}

private:
	/// <summary>
	/// For a given camera coordinate, see if a ray shot from the camera's origin through a pixel as hit a sphere located at 0,0,0
	/// </summary>
	/// <param name="coord"></param>
	/// <returns>the color of the collision; red it there was a collision, black if there was not</returns>
	glm::vec4 TraceRay(Ray& ray);

	/// <summary>
	/// Detects whether a ray has hit any object in the scene, and stores the closest result in a hit record (t value, point, and normal)
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="hitRecord"></param>
	/// <returns>Whether or not the ray hit the Scene</returns>
	bool HitScene(Ray& ray, float ray_tmin, float ray_tmax, Hit& hitRecord);

	/// <summary>
	/// Converts a color vector stored with rgb values between 0 and 1 to a 32 bit integer (RGBA format)
	/// </summary>
	/// <param name="color"></param>
	/// <returns>the 32 bit form of the color</returns>
	uint32_t ConvertColorVectorToInt(glm::vec4& color);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;


};