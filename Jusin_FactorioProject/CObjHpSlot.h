#pragma once

#include "CMonoBehaviour.h"
#include "CSpriteRenderer.h"

class CObjHpSlot :
    public CMonoBehaviour
{
public:
	struct HpSlotOptions
	{
		int maxSlotCount = 6;
		ColorValue emptyColor = ColorValue::Gray();
		ColorValue fillColor[3] = { ColorValue::Green(), ColorValue::Yellow(), ColorValue::Red() };
	};

public:
    CObjHpSlot(CGameObject* _obj);
    ~CObjHpSlot();

public:public:
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
	HpSlotOptions& getOption() { return m_sOption; }

public:
	void UpdateFill(const float _value);

private:
	CGameObject* m_pTargetObj;
	CSpriteRenderer* m_pBox;
	vector<CSpriteRenderer*> m_vSlotList;
	float m_fFill;

	HpSlotOptions m_sOption;
};

