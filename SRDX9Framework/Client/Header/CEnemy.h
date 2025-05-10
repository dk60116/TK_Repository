#pragma once

#include "Component.h"
#include "CSpriteRenderer.h"

class CEnemy :
    public CComponent
{
public:
	CEnemy();
	~CEnemy();

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

public:
	static CEnemy* Create();

public:
	void SetTarget(CTransform* _target) { m_pTarget = _target; }

public:
	CSpriteRenderer* m_pRenderer;
	float m_fMoveSpeed, m_fRotaionSpeed;

	CTransform* m_pTarget;
	_bool m_bInit;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_pRenderer)
	SERIALIZEFIELD(m_pTarget)
	END_SERIALIZEFIELD
};

