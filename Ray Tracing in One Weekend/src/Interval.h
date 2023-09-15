#pragma once

struct Interval
{
	float Min;
	float Max;

	Interval() : Min(Infinity), Max(-Infinity) {}
	Interval(float min, float max) : Min(min), Max(max) {}
	Interval(const Interval& a, const Interval& b) : Min(fmin(a.Min, b.Min)), Max(fmax(a.Max, b.Max)) {}

	bool Constains(float x) const { return Min <= x && x <= Max; }
	bool Surrounds(float x) const { return Min < x && x < Max; }

	float Clamp(float x) const
	{
		if (x < Min) return Min;
		if (x > Max) return Max;
		return x;
	}

	float Size() const
	{
		return Max - Min;
	}

	Interval Expand(float delta) const
	{
		float padding = delta / 2;
		return Interval(Min - padding, Max + padding);
	}

	static const Interval Empty, Universe;
};

const static Interval Empty(Infinity, -Infinity);
const static Interval Universe(-Infinity, Infinity);

Interval operator+(const Interval& ival, float displacement)
{
	return Interval(ival.Min + displacement, ival.Max + displacement);
}

Interval operator+(float displacement, const Interval& ival)
{
	return ival + displacement;
}
