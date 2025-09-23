#pragma once
#include "DungeonObject.h"

class CLadder final : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CLadder();
	~CLadder();

public:
	static CLadder* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnDestroy() override;

public:
	const vector3 Get_HeightValues() const;
	void Set_HeightValue(const _float _bottom, const _float _top, const _float _upPlat);

private:
	_float m_fTopHeight, m_fBottomHeight, m_fUpPlatHeight;
};

