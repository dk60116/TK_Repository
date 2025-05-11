#pragma once

#include "struct.h"

class CGameObject;

class CTransform
{
public:
	CTransform();
	~CTransform();

public:
	vector2& getPosition() { return m_v2Position; }
	void SetPosition(const vector2 _pos) { m_v2Position = _pos; };
	void AddPosition(const vector2 _pos) { m_v2Position += _pos; }
	const vector2& getScale() { return m_v2Scale; }
	void SetScale(const vector2 _scale) { m_v2Scale = _scale; };
	const vector2& getChildPosition() { return m_v2ChildPosition; }
	void SetChildPosition(const vector2 _pos) { m_v2ChildPosition = _pos; };
	void AddChildPosition(const vector2 _pos) { m_v2ChildPosition += _pos; };
	const vector2& getPivot() { return m_v2Pivot; }
	void SetPivot(const vector2 _pivot) { m_v2Pivot = _pivot; }
	void Translate(const vector2 _dir);
	void Reset();
	const vector2& getTransateDir() { return m_v2FixedDirection; }

private:
	vector2 m_v2Position, m_v2Scale, m_v2ChildPosition, m_v2Pivot;
	float m_fRotation;
	vector2 m_v2FixedDirection;
};
