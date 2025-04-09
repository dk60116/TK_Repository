#pragma once

#include "CComponent.h"
#include "CSpriteRenderer.h"

class CPlayer :
    public CComponent
{
public:
    CPlayer();
    ~CPlayer();

public:
	void Awake() override;
	void Start()  override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

private:
	void KeyInput();

public:
    CSpriteRenderer* m_pRenderer;
	float m_fMoveSpeed, m_fRotaionSpeed;
};

