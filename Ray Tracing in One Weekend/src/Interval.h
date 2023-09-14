#pragma once

struct Interval
{
	float Min;
	float Max;

	Interval() : Min(Infinity), Max(-Infinity) {}
	Interval(float min, float max) : Min(min), Max(max) {}

	bool Constains(float x) const { return Min <= x && x <= Max; }
	bool Surrounds(float x) const { return Min < x && x < Max; }

	float Clamp(float x) const
	{
		if (x < Min) return Min;
		if (x > Max) return Max;
		return x;
	}

	static const Interval Empty, Universe;
};

const static Interval Empty(Infinity, -Infinity);
const static Interval Universe(-Infinity, Infinity);
