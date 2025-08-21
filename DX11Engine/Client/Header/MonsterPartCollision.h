#pragma once

#include "Component.h"
#include "Monster.h"

class CMonsterPartCollision final : public CComponent
{
protected:
	explicit CMonsterPartCollision();
	~CMonsterPartCollision();

public:
	static CMonsterPartCollision* Create();
	CMonsterPartCollision* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Set_Monster(CMonster* _monster);
	void SetCenter(const vector3& _center);
	void SetSize(const vector3& _size);
	CMonster* Get_Monster();

public:
	CBoxCollider* Get_Collider();

private:
	CMonster* m_pMonster;
	CBoxCollider* m_pCollider;
};

