#pragma once

#include "Utils.h"

class Perlin
{
public:
	Perlin()
	{
		m_RandomVector = new glm::vec3[m_PointCount];

		for (int i = 0; i < m_PointCount; i++)
		{
			m_RandomVector[i] = glm::normalize(RandomVector(-1.0f, 1.0f));
		}

		m_PermX = PerlinGeneratePerm();
		m_PermY = PerlinGeneratePerm();
		m_PermZ = PerlinGeneratePerm();
	}

	~Perlin()
	{
		delete[] m_RandomVector;
		delete[] m_PermX;
		delete[] m_PermY;
		delete[] m_PermZ;
	}

	float Noise(const glm::vec3& point) const
	{
		float u = point.x - floor(point.x);
		float v = point.y - floor(point.y);
		float w = point.z - floor(point.z);

		int i = static_cast<int>(floor(point.x));
		int j = static_cast<int>(floor(point.y));
		int k = static_cast<int>(floor(point.z));

		glm::vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
					c[di][dj][dk] = m_RandomVector[
						m_PermX[(i + di) & 255] ^
						m_PermY[(j + dj) & 255] ^
						m_PermZ[(k + dk) & 255]
					];
				}
			}
		}

		return TrilinearInterpolation(c, u, v, w);
	}

	float Turbulence(const glm::vec3& point, int depth = 7) const
	{
		float accum = 0.0f;
		glm::vec3 tempPoint = point;
		float weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * Noise(tempPoint);
			weight *= 0.5f;
			tempPoint *= 2.0f;
		}

		return fabs(accum);
	}

private:
	static const int m_PointCount = 256;
	glm::vec3* m_RandomVector;
	int* m_PermX;
	int* m_PermY;
	int* m_PermZ;

	static int* PerlinGeneratePerm()
	{
		int* p = new int[m_PointCount];

		for (int i = 0; i < m_PointCount; i++)
		{
			p[i] = i;
		}

		Permute(p, m_PointCount);

		return p;
	}

	static void Permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = RandomInt(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static float TrilinearInterpolation(glm::vec3 c[2][2][2], float u, float v, float w)
	{
		float uu = u * u * (3 - 2 * u);
		float vv = v * v * (3 - 2 * v);
		float ww = w * w * (3 - 2 * w);
		float accum = 0.0f;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					glm::vec3 weightV(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * glm::dot(c[i][j][k], weightV);
				}
			}
		}

		return accum;
	}
};
