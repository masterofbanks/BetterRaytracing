#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>

/// <summary>
/// Class that takes a scene description as an input, and produces the pixels of an image as an output.
/// </summary>
class Renderer {
public:
	Renderer() = default;

	/// <summary>
	/// This method should get called whenever an image needs to be resized. Ex: viewport for our camera gets reized in the GUI
	/// </summary>
	/// <param name="width">New width of the image</param>
	/// <param name="height">New height of the image</param>
	void OnResize(uint32_t width, uint32_t height);



	/// <summary>
	/// Render every pixel that makes up the viewport of the image
	/// </summary>
	void Render();

	/// <summary>
	/// Getter method for the final image of the render
	/// </summary>
	/// <returns>the final image</returns>
	std::shared_ptr<Walnut::Image> GetFinalImage() {
		return m_FinalImage;
	}

private:
	uint32_t PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};