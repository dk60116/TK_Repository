#pragma once

#include "CStructure.h"
#include "CAnimator.h"
#include "CDropItem.h"

class CBeltManager;

class CBelt :
    public CStructure
{
public:
	CBelt();
	~CBelt();

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
	void UpdateAdjacentTiles();
	void UpdateSideAnimations();
	CTile* getAdjacentTile(StructureDir _dir) { return m_pAdjacentTiles[_dir]; }
	CBelt* getAdjacentBelt(StructureDir _dir) { return m_pAdjacentBelts[_dir]; }

public:
	void OnOutputHandler() override;

private:
	CAnimator* m_pBodyAnimator, * m_pSideAnmA, * m_pSideAnmB;
	CTile* m_pAdjacentTiles[COUNT];
	CBelt* m_pAdjacentBelts[COUNT];

	friend class CBeltManager;
};

