#pragma once

#include "CMonoBehaviour.h"
#include "CSpriteRenderer.h"

class CSpriteRender_Level :
    public CMonoBehaviour
{
public:
	CSpriteRender_Level();
	CSpriteRender_Level(CSpriteRenderer* _renderer);
    ~CSpriteRender_Level();

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
	void SetRenderPos(const vector2Int _start[], const vector2Int _end[]);
	void SetIsRandomX(bool _value) { m_bRandomX = _value; }

private:
    CSpriteRenderer* m_pRenderer;

	vector2Int m_v2StartPos[3];
	vector2Int m_v2EndPos[3];

	friend class SprieRenderer;
	bool m_bRandomX;
	int m_iXMax, m_iX;
};

