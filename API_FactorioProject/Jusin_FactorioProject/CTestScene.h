#pragma once

#include "CScene.h"
#include "pch.h"
#include "CTileMap.h"
#include "CPlayer.h"
#include "CUI.h"
#include "CAudioSource.h"

class CTestScene :
    public CScene
{
public:
    CTestScene();
    ~CTestScene();

    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    CPlayer& getPlayer() { return *m_pPlayer; };
    void CloseInventoryUI();

private:
    void KeyInput();

private:
    vector2 m_v2ZoomLimit;
    CTileMap* m_pDefaultMap;
    CPlayer* m_pPlayer;

    vector<CTileMap*> m_vTileArchive;

    CUI* m_pInventoryUI;
    CAudioSource* m_pSoundObj;

    vector<CGameObject*> m_vMonsterList;

    CImage* m_pEndingPanel;
    CText* m_pEndingText;
};

