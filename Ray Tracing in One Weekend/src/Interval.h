#pragma once

struct Interval
{
	double Min;
	double Max;

	Interval() : Min(Infinity), Max(-Infinity) {}
	Interval(double min, double max) : Min(min), Max(max) {}

	bool Constains(double x) const { return Min <= x && x <= Max; }
	bool Surrounds(double x) const { return Min < x && x < Max; }

	double Clamp(double x) const
	{
		if (x < Min) return Min;
		if (x > Max) return Max;
		return x;
	}

	static const Interval Empty, Universe;
};

const static Interval Empty(Infinity, -Infinity);
const static Interval Universe(-Infinity, Infinity);
