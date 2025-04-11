#pragma once

#include "CMonoBehaviour.h"
#include "CItem.h"
#include "CSpriteRenderer.h"
#include "CSpriteRender_Level.h"

class CItemManager;
class CTile;
class CBelt;

class CDropItem :
    public CMonoBehaviour
{
public:
    enum DropType { FIELD, BELT };

public:
    CDropItem(int _itemId, CTile* _tile);
    CDropItem(int _itemId, CTile* _tile, int _index);
    CDropItem(int _itemId, CBelt* _belt);
    ~CDropItem();

public:
    virtual void Awake() override;
    virtual void Start() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void OnMouseEnter() override;
    virtual void OnMouseExit() override;
    virtual void OnDestroy() override;
    void Reset() override;
    void FixedUpdate() override;
    void OnEnable() override;
    void OnDisable() override;
    void Render(HDC _hdc) override;
    void OnCollisionEnter(CCollider* _other) override;
    void OnCollisionStay(CCollider* _other) override;
    void OnCollisionExit(CCollider* _other) override;
    void OnMouseOver() override;

public:
    CItem* getItem() { return m_pItem; }
    const DropType& getType() { return m_eType; }
    CTile* getTile() { return m_pCrtTile; }
    CBelt* getCrtBelt() { return m_pCrtBelt; }
    int& getIndex() { return m_iTileIndex; }
    void SetIndex(const int _index) { m_iTileIndex = _index; }
    void ChangeTile(CTile* _tile);
    void CheckStayBelt();

private:
    DropType m_eType;
    CItem* m_pItem;
    CTile* m_pCrtTile;
    CBelt* m_pCrtBelt;
    CSpriteRenderer* m_pRender;
    CSpriteRender_Level* m_pLevel;
    int m_iTileIndex;
    bool m_bTempMoving;
};

