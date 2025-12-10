#include "Renderer.h"
#include "Walnut/Random.h"


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
	
	//loop through ys on the outside to be more friendly to the cpu cache
	//cpu can seach through the horizontally placed pixels faster
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) 
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) 
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // -1 -> 1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);

		}
	}
	
	

	m_FinalImage->SetData(m_ImageData);

}


/// <summary>
/// For a given camera coordinate, see if a ray shot from the camera's origin through a pixel as hit a sphere located at 0,0,0
/// </summary>
/// <param name="coord"></param>
/// <returns>the color of the collision; red it there was a collision, black if there was not</returns>
uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 origin = glm::vec3(0, 0, -2);
	glm::vec3 direction = glm::vec3(coord.x, coord.y, -1);

	float radius = 0.5f;
	glm::vec3 sphereCenter = glm::vec3(0, 0, 0);

	float a = glm::dot(direction, direction);
	float b = -2 * glm::dot(direction, sphereCenter - origin);
	float c = glm::dot((sphereCenter - origin), (sphereCenter - origin)) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant >= 0) {
		return 0xff0000ff;
	}

	return 0x0000000;
	
}



