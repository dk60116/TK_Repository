#pragma once

#include "Component.h"

NS_BEGIN(Engine)

NS_BEGIN(EngineAI)

class ENGINE_DLL CNavMeshAgent : public CComponent
{
	friend class CGameObject;

	struct Tri
	{
		vector3 a, b, c;
		_uint n0, n1, n2;
	};

private:
	explicit CNavMeshAgent();
	~CNavMeshAgent();

private:
	static CNavMeshAgent* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void OnDestroy() override;

public:
	void Set_NavMehsh(CNaviMesh* _walkable, CNaviMesh* _walkunable);

private:
	_bool IsInsideOrGetNeighbor(const vector3& posW, _int curPoly, _int& outNextPoly, vector3& outProj, _int* outViolatedEdge = nullptr) const;
	vector3 ProjectToPolyPlane(const vector3& pW, const CNaviMesh::NaviPolygon& poly) const;
	_bool TryAdvanceAcrossNeighbors(const vector3& posW, _int& ioPoly, vector3& ioProj) const;
	void RollbackToPrevPosition();

private:
	CNaviMesh* m_pNavMeshWalkable, * m_pNavMeshWalkUnable;
	_int m_iCurrentPoly;
};

NS_END

NS_END

