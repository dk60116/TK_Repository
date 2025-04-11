#pragma once

#include "CScene.h"

class CMianScene :
    public CScene
{
public:
    CMianScene();
    ~CMianScene();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    void GoGameScene();
};

