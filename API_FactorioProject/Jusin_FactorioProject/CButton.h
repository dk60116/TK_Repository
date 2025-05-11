#pragma once

#include "pch.h"
#include <functional>
#include "CUI.h"
#include "CImage.h"

class CButton :
    public CUI
{
public:
    CButton(wstring _imgName);
	CButton(wstring _imgName, vector2Int _end);
    ~CButton();

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
	void AddOnClickEvent(const function<void()>& _callback);
	void OnClick();
	void SetSpriteArea(vector2Int _start, vector2Int _end);

private:
    CImage* m_pImage;
	wstring m_strResourceName;
	vector2Int m_v2SpriteStart, m_v2SpriteEnd;
	bool m_bHighlight, m_bSelect;

	vector <std::function<void()>> m_vOnClickEvents;
};

