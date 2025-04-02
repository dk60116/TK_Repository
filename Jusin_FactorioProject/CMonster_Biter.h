#pragma once

#include "CMonster.h"
#include "CObjHpSlot.h"

class CMonster_Biter :
    public CMonster
{
public:
    CMonster_Biter();
    ~CMonster_Biter();

public:
    void Awake() override;
    void Reset() override;
    void Update() override;
    void FixedUpdate() override;
    void OnEnable() override;
    void OnDisable() override;
    void Render(HDC _hdc) override;
    void OnCollisionEnter(CCollider* _other) override;
    void OnCollisionStay(CCollider* _other) override;
    void OnCollisionExit(CCollider* _other) override;
    void OnMouseEnter() override;
    void OnMouseOver() override;
    void OnMouseExit() override;

public:
    void OnDead() override;

private:
    int m_iDirection;
    CObjHpSlot* m_pHpSlot;
};

