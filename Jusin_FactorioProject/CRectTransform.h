#pragma once

#include "pch.h"

struct TRect
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
};

class CRectTransform
{
public:
	CRectTransform();
	~CRectTransform();

public:
	vector2& getAnchoredPos() { return m_v2AnchoredPosition; }
	void SetAnchoredPos(const vector2 _pos) { m_v2AnchoredPosition = _pos; }
	vector2& getChildPos() { return m_v2ChildPosition; }
	void SetChildPos(const vector2 _pos) { m_v2ChildPosition = _pos; }
	const vector2& getPivot() { return m_v2Pivot; }
	void SetPivot(const vector2 _pivot) { m_v2Pivot = _pivot; }
	void SetPosition(const vector2 _pos);
	vector2& getPosition();
	vector2& getSize() { return m_v2Size; }
	void SetSize(const vector2 _size) { m_v2Size = _size; }
	const TRect getTRect();
	const vector2& getAnchorPreset() { return m_v2AnchorPreset; }
	void SetAnchorPreset(const vector2 _value) { m_v2AnchorPreset = _value; }

private:
	vector2 m_v2AnchoredPosition;
	vector2 m_v2ChildPosition;
	vector2 m_v2Size;
	vector2 m_v2Pivot;
	vector2 m_v2AnchorPreset;

	TRect m_sRect;
};

