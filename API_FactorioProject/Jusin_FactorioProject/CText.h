#pragma once

#include "CUI.h"

struct textOption
{
	wstring font = L"Arial";
    int fontSize = 20;
    int aligmentX = DT_CENTER;
    int aligmentY = DT_VCENTER;
	int lineOption = DT_SINGLELINE;
    ColorValue color = ColorValue::Black();
	bool bold = false;
	bool italic = false;
	bool underLint = false;
	bool centerLint = false;
};

class CText 
	: public CUI
{
public:
    CText();
    ~CText();

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
    void SetText(wstring _text);
    textOption& getOption() { return m_sOption; }

private:
    wstring m_strText;
    textOption m_sOption;
};

