#pragma once

#include "CUI.h"

class CImage :
    public CUI
{
public:
    CImage(wstring _spriteName);
    ~CImage();

public:
	void Awake() override;
	void Start() override;
	void Reset() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void Render(HDC _hDC) override;
	void OnCollisionEnter(CCollider* _other) override;
	void OnCollisionStay(CCollider* _other) override;
	void OnCollisionExit(CCollider* _other) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;

public:
	void SetSpriteArea(const vector2Int _start, const vector2Int _end) { m_v2SpriteStart = _start; m_v2SpriteEnd = _end; }
	void SetSprite(const wstring& _name) { m_strSpriteName = _name; }
	void SetFillAmount(vector2 _value) { m_fFillAmount = _value; }
	void SetFillAmountX(float _value) { m_fFillAmount.x = _value; }
	void SetFillAmountY(float _value) { m_fFillAmount.y = _value; }
	void SetColor(const ColorValue _color) { m_sRectColor = _color; }
	void SetAlpha(const float _value) { m_fAlpha = _value; }

private:
	wstring m_strSpriteName;
	ColorValue m_sTPColor;
	vector2Int m_v2SpriteStart, m_v2SpriteEnd;
	vector2 m_fFillAmount;
	ColorValue m_sRectColor;
	float m_fAlpha;
};

