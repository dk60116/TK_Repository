#pragma once

#include "CMonoBehaviour.h"
#include "CRectTransform.h"

class CCanvas;

class CUI :
    public CMonoBehaviour
{
public:
    CUI();
    virtual ~CUI();

public:
    void Awake() override;
    void Start() override;
    void Reset() override;
    void LateUpdate() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnDestroy() override;

public:
    void UI_Update();
    void UI_LateUpdate();

public:
    virtual void UI_OnMouseEnter();
    virtual void UI_OnMouseStay();
    virtual void UI_OnMouseExit();

public:
    void UpdateFit();
    CRectTransform& getRect() { return m_cRect; }
    CCanvas* getCanvs() { return m_pCanvas; }
    void SetCanvas(CCanvas* _canvas) { m_pCanvas = _canvas; }
    void CanvasInsert(CCanvas* _canvas);
    const bool& getOnMouse() { return m_bIsMouse; }
    const vector2Int& getMousePivot() { return m_v2MousePivot; }
    list<CUI*>& getChildList() { return m_lChildList; }

    CUI* getParent() { return m_pParentUI; }
    void SetParent(CUI* _parent);

protected:
    CRectTransform m_cRect;
    CCanvas* m_pCanvas;

    bool m_bIsMouse, m_bIsPrveMouse;
    vector2Int m_v2MousePivot;

protected:
    CUI* m_pParentUI;
    list<CUI*> m_lChildList;
};

