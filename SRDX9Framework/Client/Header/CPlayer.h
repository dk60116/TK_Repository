#pragma once

#include "Component.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"

class CPlayer :
    public CComponent
{
public:
    CPlayer();
    ~CPlayer();

public:
	static CPlayer* Create();

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
	CMeshRenderer* m_pMeshRenderer;
	_float m_fMoveSpeed, m_fRotaionSpeed;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_fMoveSpeed)
	SERIALIZEFIELD(m_fRotaionSpeed)
	END_SERIALIZEFIELD
};

