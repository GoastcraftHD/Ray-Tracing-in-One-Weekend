#pragma once

#include "Utils.h"
#include "Image.h"
#include "Perlin.h"

class Texture
{
public:
	virtual ~Texture() = default;

	virtual glm::vec4 Value(float u, float v, const glm::vec3& point) const = 0;
};

class SolidColorTexture : public Texture
{
public:
	SolidColorTexture(glm::vec4 color) : m_Color(color) {}
	SolidColorTexture(float r, float g, float b, float a) : SolidColorTexture(glm::vec4(r, g, b, a)) {}

	glm::vec4 Value(float u, float v, const glm::vec3& point) const override
	{
		return m_Color;
	}

private:
	glm::vec4 m_Color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture(float scale, std::shared_ptr<Texture> evenTexture, std::shared_ptr<Texture> oddTexture) : m_InvertedScale(1 / scale), m_EvenTexture(evenTexture), m_OddTexture(oddTexture) {}
	CheckerTexture(float scale, glm::vec4 evenColor, glm::vec4 oddColor) : m_InvertedScale(1 / scale), m_EvenTexture(std::make_shared<SolidColorTexture>(evenColor)), m_OddTexture(std::make_shared<SolidColorTexture>(oddColor)) {}

	glm::vec4 Value(float u, float v, const glm::vec3& point) const override
	{
		int xInteger = static_cast<int>(std::floor(m_InvertedScale * point.x));
		int yInteger = static_cast<int>(std::floor(m_InvertedScale * point.y));
		int zInteger = static_cast<int>(std::floor(m_InvertedScale * point.z));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? m_EvenTexture->Value(u, v, point) : m_OddTexture->Value(u, v, point);
	}

private:
	float m_InvertedScale;
	std::shared_ptr<Texture> m_EvenTexture;
	std::shared_ptr<Texture> m_OddTexture;
};

class ImageTexture : public Texture
{
public:
	ImageTexture(const char* filePath) : m_Image(filePath) {}

	glm::vec4 Value(float u, float v, const glm::vec3& point) const override
	{
		if (m_Image.Height() <= 0) return glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		u = Interval(0.0f, 1.0f).Clamp(u);
		v = 1.0f - Interval(0.0f, 1.0f).Clamp(v);

		int i = static_cast<int>(u * m_Image.Width());
		int j = static_cast<int>(v * m_Image.Height());
		const uint8_t* pixel = m_Image.PixelData(i, j);

		float colorScale = 1.0f / 255.0f;
		return glm::vec4(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2], colorScale * pixel[3]);
	}

private:
	Image m_Image;
};

class NoiseTexture : public Texture
{
public:
	NoiseTexture() {}
	NoiseTexture(float scale) : m_Scale(scale) {}

	glm::vec4 Value(float u, float v, const glm::vec3& point) const override
	{
		glm::vec3 s = m_Scale * point;
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f * (1.0f + sin(s.z + 10.0f * m_Noise.Turbulence(s)));
		//glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f * (1.0f + m_Noise.Noise(m_Scale * point));
		return glm::vec4(color, 1.0f);
	}

private:
	Perlin m_Noise;
	float m_Scale;
};
