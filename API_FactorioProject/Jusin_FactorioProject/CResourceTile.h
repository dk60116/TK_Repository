#pragma once

#include "CMonoBehaviour.h"
#include "CItem.h"

class CResourceTile :
    public CMonoBehaviour
{
public:
	enum ResourceType { WATER = 0, COAL, STONE, IRON_ORE, COPPER_ORE };

public:
    CResourceTile(ResourceType _type, int _reserve);
    ~CResourceTile();

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
	const ResourceType& getType() { return m_eType; }
	void AddHp(float _value);
	const float& getHp() { return m_fCurrentHp; }
	void GetReward();

private:
	ResourceType m_eType;
	int m_iReserves;
	float m_fCurrentHp;
};

