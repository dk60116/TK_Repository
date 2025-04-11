#pragma once

#include "CMonoBehaviour.h"

class CSpriteRender_Level;

enum SpriteRenderMode { RectDraw, CircleDraw, BMP, TBMP, BMPAnimation };

class CAnimator;

class CSpriteRenderer :
    public CMonoBehaviour
{
public:
	CSpriteRenderer();
	CSpriteRenderer(wstring _resourceName, vector2Int _endPoint, bool _transparent = false);
	CSpriteRenderer(CAnimator* _animator);
	~CSpriteRenderer();

public:
	void Awake() override;
	void Start() override;
	void Reset() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void Render(HDC _hDC) override;
	void OnCollisionEnter(CCollider* _other) override;
	void OnCollisionStay(CCollider* _other) override;
	void OnCollisionExit(CCollider* _other) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;

public:
	void RenderRect(HDC _hDC);
	void RenderCircle(HDC _hDC);
	void RenderBMP(HDC _hDC, vector2Int _start, vector2Int _end);
	void RenderTBMP(HDC _hDC, vector2Int _start, vector2Int _end);
	void RenderBMPAnimation(HDC _hDC);

public:
	const SpriteRenderMode& getMode() { return m_eMode; }
	void SetMode(SpriteRenderMode _mode) { m_eMode = _mode; }
	void SetSprite(const wstring _strName) { m_strResourceName = _strName; }
	void SetAnimator(CAnimator* _animator) { m_pAnimator = _animator; }
	void SetColorTint(const ColorValue _color) { m_sColorTint = _color; }
	void SetTPColor(const ColorValue _value) { m_sTPColor = _value; }
	void SetScale(const float _value) { m_fScale = _value; }
	void SetSpriteArea(const vector2Int _start, const vector2Int _end);
	const float& getAlpha() { return m_fAlpha; }
	void SetAlpha(const float _value) { m_fAlpha = _value; }

private:
	SpriteRenderMode m_eMode;
	wstring m_strResourceName;
	CAnimator* m_pAnimator;
	vector2Int m_v2StartPoint, m_v2EndPoint;
	ColorValue m_sColorTint;
	ColorValue m_sTPColor;
	float m_fScale;
	float m_fAlpha;

	friend class CSpriteRender_Level;
};

