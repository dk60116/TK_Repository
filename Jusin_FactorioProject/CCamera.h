#pragma once

#include "CMonoBehaviour.h"

class CCamera :
	public CMonoBehaviour
{
public:
	CCamera();
	~CCamera();

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
	const CGameObject* getTarget() { return m_pTarget; }
	void SetTarget(CGameObject* _target) { m_pTarget = _target; }
	const vector2& getMove();
	const vector2& getOffset() { return m_v2Offset; }
	void SetOffset(vector2 _offset) { m_v2Offset = _offset; }
	const float& getSize() { return m_fSize; }
	void SetSize(float _value) { m_fSize = _value; }
	const vector2 GetCameraPosToWord();
	const vector2 GetMousePosToWorld();

private:
	CGameObject* m_pTarget;
	vector2 m_v2Offset;
	float m_fSize;
	ColorValue m_sBackgroundColor;
};

