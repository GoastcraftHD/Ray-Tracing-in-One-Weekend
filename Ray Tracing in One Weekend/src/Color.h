#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "Vec3.h"

using Color = Vec3;

inline double LinearToGamma(double linearComponent)
{
	return sqrt(linearComponent);
}

std::string stringColor(Color pixelColor, int samplesPerPixel)
{
	double r = pixelColor.x();
	double g = pixelColor.y();
	double b = pixelColor.z();

	double scale = 1.0 / samplesPerPixel;
	r *= scale;
	g *= scale;
	b *= scale;

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.0, 0.999);

	std::stringstream ss;
	ss << static_cast<int>(256 * intensity.Clamp(r)) << " " << static_cast<int>(256 * intensity.Clamp(g)) << " " << static_cast<int>(256 * intensity.Clamp(b)) << "\n";
	
	return ss.str();
}

uint32_t PackColor(Color pixelColor, int samplesPerPixel)
{
	double r = pixelColor.x();
	double g = pixelColor.y();
	double b = pixelColor.z();

	double scale = 1.0 / samplesPerPixel;
	r *= scale;
	g *= scale;
	b *= scale;

	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);

	static const Interval intensity(0.0, 0.999);

	uint32_t packedColor = 0;
	packedColor |= 255 << 24;
	packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(b)) << 16;
	packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(g)) << 8;
	packedColor |= static_cast<uint8_t>(256 * intensity.Clamp(r));

	return packedColor;
}
