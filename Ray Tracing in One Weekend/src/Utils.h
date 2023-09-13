#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

const double Infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees)
{
	return degrees * Pi / 180.0;
}

inline double RandomDouble() { return rand() / (RAND_MAX + 1.0); }
inline double RandomDouble(double min, double max) { return min + (max - min) * RandomDouble(); }

#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"