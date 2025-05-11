#pragma once

#include "CMonoBehaviour.h"
#include "CItemManager.h"
#include "CTile.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CCollider.h"

class CStructure :
    public CMonoBehaviour
{
public:
    enum StructureDirType { NONE, RLUD, UD, RL, TYPE_END, EIGHT };
    enum StructureDir { RIGHT, LEFT, UP, DOWN, COUNT };

public:
    CStructure();
    virtual ~CStructure();

public:
    void StructureStart();

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
    CItem* getItem() { return m_pItem; }
    void SetItem(CItem* _item) { m_pItem = _item; }
    CTile* getFirstTile() { return m_vTile[0]; }
    void SetTile(CTile* _tile);
    const float& getCollectingHp() { return m_fCollectingGauge; }
    void AddCollectingHp(float _value);
    void GetItem();
    void SetDirection(const StructureDir _dir) { m_eDir = _dir; }
    void SetEDirection(const BYTE _dir) { m_iDir = _dir; }
    const StructureDirType& getDirType() { return m_eDirType; }
    const StructureDir& getDirection() { return m_eDir; }
    const BYTE& getIDir() { return m_iDir; }

    virtual void OnOutputHandler() PURE;

protected:
    CItem* m_pItem;
    vector<CTile*> m_vTile;
    CSpriteRenderer* m_pRenderer;
    CAnimator* m_pAnimator;
    float m_fCollectingGauge;
    CCollider* m_pCollider;
    StructureDirType m_eDirType;
    StructureDir m_eDir;
    BYTE m_iDir;
};

