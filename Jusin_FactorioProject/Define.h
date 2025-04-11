#pragma once

#include "pch.h"
#include "struct.h"
#include "CTransform.h"

#define UNIT 50.f
#define PURE = 0
#define DELTA_TIME (float)CTime::GetInstance().getDeltaTime()
#define FIXED_TIMESTEP 0.02f

#define PI 3.141592f
#define XM_2PI 6.283185307f

#define SingletonClass(c) \
private: \
    c(); \
    c(const c& rhs) = delete; \
    c& operator=(const c& rhs) = delete; \
    ~c(); \
public: \
    static c& GetInstance() \
    { \
        static c instance; \
        return instance; \
    } \
private: \
    enum { __singleton_dummy = 0 }

template <typename T>
T clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max ? max : value);
}

template<typename T>
void Safe_Delete(T& p)
{
    if (p)
    {
        delete p;
        p = nullptr;
    }
}

#define TRAVERSAL_ITER(p, n) auto n = p.begin(); n != p.end(); ++n

static void DrawRectangle(HDC _hDC, vector2 _center, vector2 _scale, CTransform* _transform, ColorValue _lineColor = ColorValue::Black(), ColorValue _faceColor = ColorValue::White(), vector2 _pivot = vector2::zero())
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(_lineColor.r, _lineColor.g, _lineColor.b));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(_faceColor.r, _faceColor.g, _faceColor.b));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);

	Rectangle(_hDC,
		int((int)_center.x - (_scale.x * 0.5f)) + (int)_transform->getPosition().x + (int)_pivot.x,
		int((int)_center.y - (_scale.y * 0.5f)) + (int)_transform->getPosition().y + (int)_pivot.y,
		int((int)_center.x + (_scale.x * 0.5f)) + (int)_transform->getPosition().x + (int)_pivot.x,
		int((int)_center.y + (_scale.y * 0.5f)) + (int)_transform->getPosition().y + (int)_pivot.y);

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);

	DeleteObject(hBrush);
	DeleteObject(hPen);
};

static void DrawTPRectangle(HDC _hDC, vector2 _center, vector2 _scale, CTransform* _transform, ColorValue _lineColor = ColorValue::Black(), vector2 _pivot = vector2::zero())
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(_lineColor.r, _lineColor.g, _lineColor.b));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));

	Rectangle(_hDC,
		int((int)_center.x - (int)(_scale.x * 0.5f)) + (int)_transform->getPosition().x + (int)_pivot.x,
		int((int)_center.y - (int)(_scale.y * 0.5f)) + (int)_transform->getPosition().y + (int)_pivot.y,
		int((int)_center.x + (int)(_scale.x * 0.5f)) + (int)_transform->getPosition().x + (int)_pivot.x,
		int((int)_center.y + (int)(_scale.y * 0.5f)) + (int)_transform->getPosition().y + (int)_pivot.y);

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);

	DeleteObject(hPen);
};

static void DrawCircle(HDC _hDC, vector2 _center, vector2 _scale, CTransform _transform, ColorValue _lineColor = ColorValue::Black(), ColorValue _faceColor = ColorValue::White(), vector2 _pivot = vector2::zero())
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(_lineColor.r, _lineColor.g, _lineColor.b));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(_faceColor.r, _faceColor.g, _faceColor.b));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);

	Ellipse
	(
		_hDC,
		int(_center.x - (_scale.x * 0.5f) + _transform.getPosition().x + _pivot.x),
		int(_center.y - (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y),
		int(_center.x + (_scale.x * 0.5f) + _transform.getPosition().x + _pivot.x),
		int(_center.y + (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y)
	);

	float c = _center.y - (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y;

	SelectObject(_hDC, hOldPen);
	SelectObject(_hDC, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

static void DrawTPCircle(HDC _hDC, vector2 _center, vector2 _scale, CTransform _transform, ColorValue _lineColor = ColorValue::Black(), ColorValue _faceColor = ColorValue::White(), vector2 _pivot = vector2::zero())
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(_lineColor.r, _lineColor.g, _lineColor.b));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(NULL_BRUSH));

	Ellipse
	(
		_hDC,
		int(_center.x - (_scale.x * 0.5f) + _transform.getPosition().x + _pivot.x),
		int(_center.y - (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y),
		int(_center.x + (_scale.x * 0.5f) + _transform.getPosition().x + _pivot.x),
		int(_center.y + (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y)
	);

	float c = _center.y - (_scale.y * 0.5f) + _transform.getPosition().y + _pivot.y;

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);

	DeleteObject(hPen);
}

static void DrawLine(HDC _hDC, vector2 _start, vector2 _end, CTransform _transform, ColorValue _lineColor = ColorValue::Black(), int _width = 1)
{
	HPEN hPen = CreatePen(PS_SOLID, _width, RGB(_lineColor.r, _lineColor.g, _lineColor.b));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	MoveToEx(_hDC, int(_start.x) + (int)_transform.getPosition().x, int(_start.y) + (int)_transform.getPosition().y, nullptr);
	LineTo(_hDC, int(_end.x) + (int)_transform.getPosition().x, int(_end.y) + (int)_transform.getPosition().y);

	SelectObject(_hDC, hOldPen);

	DeleteObject(hPen);
}
