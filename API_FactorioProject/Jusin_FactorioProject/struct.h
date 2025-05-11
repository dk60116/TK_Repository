#pragma once

#include "cmath"

using namespace std;

struct vector2Int;

struct vector2
{
	float x, y;

	vector2() :x(0.f), y(0.f) {};
	vector2(float _x, float _y) :x(_x), y(_y) {}
	vector2(int _x, int _y) :x((float)_x), y((float)_y) {}

	static vector2 zero()
	{
		return vector2(0.f, 0.f);
	}

	static vector2 one()
	{
		return vector2(1.f, 1.f);
	}

	static vector2 up()
	{
		return vector2(0.f, -1.f);
	}

	static vector2 down()
	{
		return vector2(0.f, 1.f);
	}

	static vector2 left()
	{
		return vector2(-1.f, 0.f);
	}

	static vector2 right()
	{
		return vector2(1.f, 0.f);
	}

	static const  float Distance(const vector2 _left, const vector2 _right)
	{
		float dx = _left.x - _right.x;
		float dy = _left.y - _right.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	const float length() const
	{
		return sqrt(x * x + y * y);
	}

	vector2 normalize() const
	{
		float mag = length();

		if (mag != 0.f)
			return vector2(x / mag, y / mag);
		else
			return zero();

		return zero();
	}

	void operator*=(const float _value)
	{
		this->x *= _value;
		this->y *= _value;
	}

	void operator*=(const int _value)
	{
		this->x *= (float)_value;
		this->y *= (float)_value;
	}

	void operator/=(const float _value)
	{
		this->x /= _value;
		this->y /= _value;
	}

	void operator/=(const int _value)
	{
		this->x /= (float)_value;
		this->y /= (float)_value;
	}

	vector2 operator*(const float _value) const
	{
		return vector2(x * _value, y * _value);
	}

	vector2 operator/(const float _value) const
	{
		return vector2(x / _value, y / _value);
	}

	vector2 operator +(const vector2 _rhs) const
	{
		return vector2(x + _rhs.x, y + _rhs.y);
	}

	vector2 operator -(const vector2 _rhs) const
	{
		return vector2(x - _rhs.x, y - _rhs.y);
	}

	vector2 operator *(const vector2 _rhs) const
	{
		return vector2(x * _rhs.x, y * _rhs.y);
	}

	vector2 operator /(const vector2 _rhs) const
	{
		return vector2(x / _rhs.x, y / _rhs.y);
	}

	const bool operator ==(const vector2 _rhs) const
	{
		if (this->x == _rhs.x && this->y == _rhs.y)
			return true;

		return false;
	}

	const bool operator !=(const vector2 _rhs) const
	{
		if (this->x == _rhs.x && this->y == _rhs.y)
			return false;

		return true;
	}

	vector2 operator +=(const vector2 _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;

		return *this;
	}

	vector2 operator -=(const vector2 _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;

		return *this;
	}

	vector2 operator *=(const vector2 _rhs)
	{
		x *= _rhs.x;
		y *= _rhs.y;

		return *this;
	}

	vector2 operator /=(const vector2 _rhs)
	{
		x /= _rhs.x;
		y /= _rhs.y;

		return *this;
	}

	const vector2Int toVector2Int() const;
};

struct vector2Int
{
	int x, y;

	vector2Int() :x(0), y(0) {};
	vector2Int(int _x, int _y) :x(_x), y(_y) {}

	static vector2Int zero()
	{
		return vector2Int(0, 0);
	}

	static vector2Int one()
	{
		return vector2Int(1, 1);
	}

	static vector2Int up()
	{
		return vector2Int(0, -1);
	}

	static vector2Int down()
	{
		return vector2Int(0, 1);
	}

	static vector2Int left()
	{
		return vector2Int(-1, 0);
	}

	static vector2Int right()
	{
		return vector2Int(1, 0);
	}

	static const float Distance(const vector2Int _left, const vector2Int _right)
	{
		int dx = _left.x - _right.x;
		int dy = _left.y - _right.y;
		return (float)sqrt(dx * dx + dy * dy);
	}

	const float length() const
	{
		return (float)sqrt(x * x + y * y);
	}

	vector2Int operator*=(const int _value)
	{
		x *= _value;
		y *= _value;
		return *this;
	}

	vector2Int operator/=(const int _value)
	{
		x /= _value;
		y /= _value;
		return *this;
	}

	vector2Int operator*(const int _value) const
	{
		return vector2Int(x * _value, y * _value);
	}

	vector2Int operator/(const int _value) const
	{
		return vector2Int(x / _value, y / _value);
	}

	vector2Int operator +(const vector2Int _rhs) const
	{
		return vector2Int(x + _rhs.x, y + _rhs.y);
	}

	vector2Int operator -(const vector2Int _rhs) const
	{
		return vector2Int(x - _rhs.x, y - _rhs.y);
	}

	vector2Int operator *(const vector2Int _rhs) const
	{
		return vector2Int(x * _rhs.x, y * _rhs.y);
	}

	vector2Int operator /(const vector2Int _rhs) const
	{
		return vector2Int(x / _rhs.x, y / _rhs.y);
	}

	const bool operator ==(const vector2Int _rhs) const
	{
		return x == _rhs.x && y == _rhs.y;
	}

	const bool operator !=(const vector2Int _rhs) const
	{
		return !(*this == _rhs);
	}

	vector2Int operator +=(const vector2Int _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		return *this;
	}

	vector2Int operator -=(const vector2Int _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		return *this;
	}

	vector2Int operator *=(const vector2Int _rhs)
	{
		x *= _rhs.x;
		y *= _rhs.y;
		return *this;
	}

	vector2Int operator /=(const vector2Int _rhs)
	{
		x /= _rhs.x;
		y /= _rhs.y;
		return *this;
	}

	//
	vector2Int operator*(const float _value) const
	{
		return vector2Int(x * (int)_value, y * (int)_value);
	}

	vector2Int operator/(const float _value) const
	{
		return vector2Int(int(x / _value), int(y / _value));
	}

	vector2Int operator +(const vector2 _rhs) const
	{
		return vector2Int(x + (int)_rhs.x, y + (int)_rhs.y);
	}

	vector2Int operator -(const vector2 _rhs) const
	{
		return vector2Int(x - (int)_rhs.x, y - (int)_rhs.y);
	}

	vector2Int operator *(const vector2 _rhs) const
	{
		return vector2Int(x * (int)_rhs.x, y * (int)_rhs.y);
	}

	vector2Int operator /(const vector2 _rhs) const
	{
		return vector2Int(x / (int)_rhs.x, y / (int)_rhs.y);
	}

	const bool operator ==(const vector2 _rhs) const
	{
		return x == (int)_rhs.x && y == (int)_rhs.y;
	}

	const bool operator !=(const vector2 _rhs) const
	{
		return !(*this == _rhs.toVector2Int());
	}

	vector2Int operator +=(const vector2 _rhs)
	{
		x += (int)_rhs.x;
		y += (int)_rhs.y;
		return *this;
	}

	vector2Int operator -=(const vector2 _rhs)
	{
		x -= (int)_rhs.x;
		y -= (int)_rhs.y;
		return *this;
	}

	vector2Int operator *=(const vector2 _rhs)
	{
		x *= (int)_rhs.x;
		y *= (int)_rhs.y;
		return *this;
	}

	vector2Int operator /=(const vector2 _rhs)
	{
		x /= (int)_rhs.x;
		y /= (int)_rhs.y;
		return *this;
	}

	//

	const vector2 toVector2() const
	{
		return vector2(static_cast<float>(x), static_cast<float>(y));
	}
};

struct ColorValue
{
	unsigned char r, g, b;

	ColorValue()
		: r(0)
		, g(0)
		, b(0)
	{
	}

	ColorValue(int _r, int _g, int _b)
	{
		r = (unsigned char)_r;
		g = (unsigned char)_g;
		b = (unsigned char)_b;
	}

	static ColorValue Black()
	{
		return ColorValue(0, 0, 0);
	}

	static ColorValue White()
	{
		return ColorValue((unsigned char)255, (unsigned char)255, (unsigned char)255);
	}

	static ColorValue Red()
	{
		return ColorValue((unsigned char)255, (unsigned char)0, (unsigned char)0);
	}

	static ColorValue Green()
	{
		return ColorValue((unsigned char)0, (unsigned char)255, (unsigned char)0);
	}

	static ColorValue Blue()
	{
		return ColorValue((unsigned char)0, (unsigned char)0, (unsigned char)255);
	}

	static ColorValue Yellow()
	{
		return ColorValue((unsigned char)255, (unsigned char)255, (unsigned char)0);
	}

	static ColorValue Magenta()
	{
		return ColorValue((unsigned char)255, (unsigned char)0, (unsigned char)255);
	}

	static ColorValue Turquoise()
	{
		return ColorValue((unsigned char)0, (unsigned char)255, (unsigned char)255);
	}

	static ColorValue Gray()
	{
		return ColorValue((unsigned char)128, (unsigned char)128, (unsigned char)128);
	}

	const bool operator ==(const ColorValue _rhs) const
	{
		return (r == _rhs.r && g == _rhs.g && b == _rhs.b);
	}

	ColorValue operator +(const ColorValue _rhs) const
	{
		int rr = r + _rhs.r;
		int gg = g + _rhs.g;
		int bb = b + _rhs.b;

		if (rr < 0)
			rr = 0;
		if (gg < 0)
			gg = 0;
		if (bb < 0)
			bb = 0;

		if (rr > 255)
			rr = 255;
		if (gg > 255)
			gg = 255;
		if (bb > 255)
			bb = 255;

		return ColorValue(rr, gg, bb);
	}

	ColorValue operator -(const ColorValue _rhs) const
	{
		int rr = r - _rhs.r;
		int gg = g - _rhs.g;
		int bb = b - _rhs.b;

		if (rr < 0)
			rr = 0;
		if (gg < 0)
			gg = 0;
		if (bb < 0)
			bb = 0;

		if (rr > 255)
			rr = 255;
		if (gg > 255)
			gg = 255;
		if (bb > 255)
			bb = 255;

		return ColorValue(rr, gg, bb);
	}
};

