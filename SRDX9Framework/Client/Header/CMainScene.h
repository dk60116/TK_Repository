#pragma once

#include "CScene.h"
#include "CLight.h"
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
    CPlayer* m_pPlayer;
    CEnemy* m_pEnemy;
};

