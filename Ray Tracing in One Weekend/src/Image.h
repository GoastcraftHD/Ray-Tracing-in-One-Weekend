#pragma once

#include <stb_image.h>
#include <cstdlib>
#include <iostream>

class Image
{
public:
	Image() : m_Data(nullptr) {}

	Image(const char* filePath)
	{
		int n; // Not used
		m_Data = stbi_load(filePath, &m_ImageWidth, &m_ImageHeight, &n, m_BytesPerPixel);
		m_BytesPerScanline = m_ImageWidth * m_BytesPerPixel;
	}

	~Image() { stbi_image_free(m_Data); }

	int Width() const { return m_Data == nullptr ? 0 : m_ImageWidth; }
	int Height() const { return m_Data == nullptr ? 0 : m_ImageHeight; }

	const uint8_t* PixelData(int x, int y) const
	{
		static uint8_t magenta[] = { 255, 0, 255 };

		if (m_Data == nullptr) return magenta;

		x = Clamp(x, 0, m_ImageWidth);
		y = Clamp(y, 0, m_ImageHeight);

		return m_Data + y * m_BytesPerScanline + x * m_BytesPerPixel;
	}

private:
	const int m_BytesPerPixel = 4;
	uint8_t* m_Data;
	int m_ImageWidth;
	int m_ImageHeight;
	int m_BytesPerScanline;

	static int Clamp(int x, int low, int high)
	{
		if (x < low) return low;
		if (x < high) return x;
		return high - 1;
	}
};
