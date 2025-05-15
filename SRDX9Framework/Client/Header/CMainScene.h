#pragma once

#include "Scene.h"
#include "Light.h"
#include "MainCamera.h"
#include "CPlayer.h"
#include "CEnemy.h"

class CMainScene :
    public CScene
{
public:
    CMainScene();
    ~CMainScene();

public:
    void EditorInit() override;
    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void Render_Game() override;
    void Destroy() override;

private:
    CMainCamera* m_pMainCam;
    CPlayer* m_pPlayer;
    CEnemy* m_pEnemy;
    CGameObject* m_pCameraParent;
};

