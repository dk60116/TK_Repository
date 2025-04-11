#pragma once

#include "CMonoBehaviour.h"
#include "CStructure.h"
#include "CItemManager.h"

struct IOItem
{
    int itemId = -1;
    int itemCount = 1;

    bool operator<(const IOItem& other) const
    {
        return std::tie(itemId, itemCount) < std::tie(other.itemId, other.itemCount);
    }
};

struct IOOption
{
    int taskOutCount = 1;
    float taskSpeed = 100.f;
    float taskMax = 50.f;
    float fuelSpeed = 10.f;
    float fuelMax = 100.f;
    map<IOItem, IOItem> ioItems = {};
};

class CInOutStructure :
    public CMonoBehaviour
{
public:
    enum IOStructureType { ALL, INPUT, FUEL, FUEL_OUT, IN_OUT};

public:
    CInOutStructure();
    ~CInOutStructure();

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
    CStructure* getStructer() { return m_pStructure; }
    const IOStructureType& getType() { return m_eType; }
    void SetType(const IOStructureType _type) { m_eType = _type; }

    const bool& getTasking() { return m_bIsTasking; }

    void SetStructure(CStructure* _structure) { m_pStructure = _structure; }
    CItem* getInputItem() { return m_pInputItem; }
    void SetInputItem(CItem* _item);
    CItem* getOutputItem() { return m_pOutputItem; }
    void SetOutputItem(CItem* _item) { m_pOutputItem = _item; }
    CItem* getFuelItem() { return m_pFuelItem; }
    void SetFuelItem(CItem* _item) { m_pFuelItem = _item; }
    void AddIOItemPair(const vector2Int _input, const vector2Int _output);

    void ReturnInputItem();
    void ReturnOutputItem();
    void RetrunFuelItem();

    const float& getProgress() { return m_fTaskProgress; }
    const float& getFuelLife() { return m_fFuelLife; }

    IOOption& getOption() { return m_sTaskOption; }

private:
    CStructure* m_pStructure;
    IOStructureType m_eType;
    bool m_bIsTasking, m_bTempFuel;
    CItem* m_pInputItem;
    CItem* m_pOutputItem;
    CItem* m_pFuelItem;
    int m_ipInputItemMax, m_ipFuelItemMax, m_iOutputItemMax;
    float m_fTaskProgress, m_fFuelLife;
    IOOption m_sTaskOption;
};

