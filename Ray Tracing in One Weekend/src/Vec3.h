#pragma once

#include <cmath>
#include <iostream>

class Vec3
{
public:
	double e[3];

	Vec3() : e{0.0, 0.0, 0.0} {}
	Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];

		return *this;
	}

	Vec3& operator*=(double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;

		return *this;
	}

	Vec3& operator/=(double t)
	{
		return *this *= 1 / t;
	}

	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double length() const
	{
		return sqrt(length_squared());
	}

	bool NearZero() const
	{
		double s = 1e-8;
		return fabs(e[0]) < s && fabs(e[1]) < s && fabs(e[2]) < s;
	}

	static Vec3 Random() { return Vec3(RandomDouble(), RandomDouble(), RandomDouble()); }
	static Vec3 Random(double min, double max) { return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)); }
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.e[0],t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 UnitVector(const Vec3& v)
{
	return v / v.length();
}

inline Vec3 RandomInUnitDisk()
{
	while (true)
	{
		Vec3 p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.length_squared() < 1)
			return p;
	}
}

inline Vec3 RandomInUnitSphere()
{
	while(true)
	{
		Vec3 p = Vec3::Random(-1, 1);

		if (p.length_squared() < 1)
			return p;
	}
}

inline Vec3 RandomUnitVector()
{
	return UnitVector(RandomInUnitSphere());
}

inline Vec3 RandomOnHemisphere(const Vec3& normal)
{
	Vec3 onUnitSphere = RandomUnitVector();

	if (Dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;
	
	return -onUnitSphere;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n) { return v - 2 * Dot(v, n) * n; }

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat)
{
	double cosTheta = fmin(Dot(-uv, n), 1.0);
	Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
	Vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.length_squared())) * n;

	return rOutPerp + rOutParallel;
}