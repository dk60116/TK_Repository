#pragma once
#include "DungeonObject.h"

class CMovingPlat final : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CMovingPlat();
	~CMovingPlat();

public:
	static CMovingPlat* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void SetMoveSpeed(const _float _value);
	void AddRout(const vector3 _pos);

private:
	vector<vector3> m_vRout;

	_bool m_bPlayerEnter;
	_float m_fMoveSpeed;
	_ubyte m_iTargetStep;
	_bool m_bBackStep;
	_float m_fWaitTime;
};

