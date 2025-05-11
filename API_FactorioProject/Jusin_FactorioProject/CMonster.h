#pragma once

#include "CMonoBehaviour.h"
#include "CAnimator.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CObjHpSlot.h"

class CMonsterManger;

struct MonsterStat
{
    float maxHp = 100.f;
    float crtHp = 0.f;
    float moveSpeed = 0.45f;
};

class CMonster :
    public CMonoBehaviour
{
public:
    CMonster();
    ~CMonster();

public:
    void Start() override;
    void LateUpdate() override;
    void OnDestroy() override;
    
public:
    virtual void OnDead() PURE;

public:
    bool& getAlibe() { return m_bAlibe; }
    void AddHp(float _dmg);

protected:
    CSpriteRenderer* m_pRender;
    CAnimator* m_pAnim;
    CAudioSource* m_pSoundObj;
    CGameObject* m_pTargetObj;
    MonsterStat m_sStat;
    bool m_bAlibe, m_bPrevAlibe;
    bool m_bIsAttack;
    CObjHpSlot* m_pHpSlot;
    float m_fAttackLockTime;
};

