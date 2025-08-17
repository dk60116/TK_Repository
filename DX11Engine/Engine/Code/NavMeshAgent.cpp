#include "epch.h"
#include "NavMeshAgent.h"

using namespace EngineAI;

CNavMeshAgent::CNavMeshAgent()
	: m_pNavMeshWalkable(nullptr)
	, m_pNavMeshWalkUnable(nullptr)
	, m_iCurrentPoly(-1)
{
}

CNavMeshAgent::~CNavMeshAgent()
{
}

CNavMeshAgent* CNavMeshAgent::Create()
{
    return new CNavMeshAgent();
}

CComponent* CNavMeshAgent::Clone() const
{
	CNavMeshAgent* clone = new CNavMeshAgent();

	clone->Set_NavMehsh(m_pNavMeshWalkable, m_pNavMeshWalkUnable);

	return clone;
}

HRESULT CNavMeshAgent::Initialize()
{
    return S_OK;
}

void CNavMeshAgent::Awake()
{
	if (!m_pNavMeshWalkable)
	{
		auto navs = CSceneManager::Get_CrtScene()->Get_NavMeshes();
		Set_NavMehsh(navs[0], navs[1]);
	}

	if (!m_pNavMeshWalkable)
		return;

	if (m_pNavMeshWalkable) 
	{
		auto* tr = m_pGameObject->Get_Transform();
		vector3 pos = tr->Get_Position();
		m_iCurrentPoly = m_pNavMeshWalkable->FindContainingPolygon(pos);
		
		if (m_iCurrentPoly < 0) 
		{
			const auto& polys = m_pNavMeshWalkable->Get_Polygons();
			if (!polys.empty()) 
			{
				_float bestD2 = FLT_MAX;
				_int best = -1;
				vector3 bestQ{};
				for (size_t i = 0; i < polys.size(); ++i) 
				{
					vector3 q = m_pNavMeshWalkable->ProjectPointToPoly(pos, (_uint)i);
					_float d2 = (q - pos).lengthSq();
					
					if (d2 < bestD2) 
					{ 
						bestD2 = d2;
						best = (_int)i;
						bestQ = q; 
					}
				}
				if (best >= 0) 
				{
					m_iCurrentPoly = best;
					tr->Set_Position(bestQ);
				}
			}
		}
	}
}

void CNavMeshAgent::Update()
{
	if (!m_pNavMeshWalkable)
		return;

	auto* tr = m_pGameObject->Get_Transform();
	vector3 posW = tr->Get_Position();

	// 1) 현재 폴리곤이 유효한가? (유효하지 않다면 빠르게 재탐색)
	if (m_iCurrentPoly < 0)
	{
		m_iCurrentPoly = m_pNavMeshWalkable->FindContainingPolygon(posW);
		if (m_iCurrentPoly < 0)
		{
			RollbackToPrevPosition();
			return;
		}
	}

	// 2) 현재 폴리곤 → 이웃을 따라 들어갈 수 있는지 검사
	vector3 proj{};
	_int poly = m_iCurrentPoly;
	const _bool ok = TryAdvanceAcrossNeighbors(posW, poly, proj);

	if (ok)
	{
		// 성공: 현재 폴리곤 갱신 + (선택) Y 높이 보정
		m_iCurrentPoly = poly;

		// 높이 보정(샘플처럼 OnCell): 원치 않으면 주석 처리
		const auto& polys = m_pNavMeshWalkable->Get_Polygons();
		vector3 onPlane = ProjectToPolyPlane(posW, polys[poly]);

		// 월드 위치 세팅 (부모 고려)
		_matrix W = tr->Get_WorldMatrix();
		_vector S, R, T;
		XMMatrixDecompose(&S, &R, &T, W);
		W = XMMatrixScalingFromVector(S) * XMMatrixRotationQuaternion(R) * XMMatrixTranslation(onPlane.x, onPlane.y, onPlane.z);
		tr->SetTransformForMatrix(W);
	}
	else
	{
		RollbackToPrevPosition();
	}
}

void CNavMeshAgent::LateUpdate()
{
}

void CNavMeshAgent::Render()
{
}

void CNavMeshAgent::OnDestroy()
{
	Safe_Release(m_pNavMeshWalkable);
	Safe_Release(m_pNavMeshWalkUnable);
}

void CNavMeshAgent::Set_NavMehsh(CNaviMesh* _walkable, CNaviMesh* _walkunable)
{
	Safe_Release(m_pNavMeshWalkable);
	Safe_Release(m_pNavMeshWalkUnable);

	m_pNavMeshWalkable = _walkable;

	if (m_pNavMeshWalkable)
		m_pNavMeshWalkable->AddRef();

	m_pNavMeshWalkUnable = _walkunable;

	if (m_pNavMeshWalkUnable)
		m_pNavMeshWalkUnable->AddRef();
}

_bool CNavMeshAgent::IsInsideOrGetNeighbor(const vector3& posW, _int curPoly, _int& outNextPoly, vector3& outProj, _int* outViolatedEdge) const
{
	outNextPoly = -1;

	if (!m_pNavMeshWalkable) 
		return false;

	const auto& polys = m_pNavMeshWalkable->Get_Polygons();
	if (curPoly < 0 || curPoly >= (_int)polys.size()) 
		return false;

	const auto& poly = polys[curPoly];
	const vector3& A = poly.vertices[0];
	const vector3& B = poly.vertices[1];
	const vector3& C = poly.vertices[2];

	// 폴리곤 평면 법선
	vector3 n = (B - A).cross(C - A);
	const _float nlen2 = n.lengthSq();
	if (nlen2 < 1e-10f) 
		return false;
	n = n * (1.0f / sqrtf(nlen2));

	// pos를 평면에 투영
	_float dist = (posW - A).dot(n);
	vector3 P = posW - n * dist;
	outProj = P;

	// 엣지별 inside 테스트 (A->B, B->C, C->A)
	auto side = [&](const vector3& P, const vector3& X, const vector3& Y)->_float
		{
		vector3 c = (Y - X).cross(P - X);
		return c.dot(n);
		};

	const _float EPS = 1e-5f;
	_float d0 = side(P, A, B); // AB
	_float d1 = side(P, B, C); // BC
	_float d2 = side(P, C, A); // CA

	const _bool inside = (d0 >= -EPS) && (d1 >= -EPS) && (d2 >= -EPS);
	if (inside) {
		if (outViolatedEdge) *outViolatedEdge = -1;
		return true;
	}

	// 어느 엣지를 넘었는지 찾아서 대응 이웃으로 이동
	_int edge = -1;
	if (d0 < -EPS) edge = 0;     // AB
	else if (d1 < -EPS) edge = 1; // BC
	else if (d2 < -EPS) edge = 2; // CA

	if (outViolatedEdge) *outViolatedEdge = edge;

	if (edge >= 0 && edge < (int)poly.neighbors.size()) {
		_uint neigh = poly.neighbors[edge];
		if (neigh != UINT_MAX)
		{
			outNextPoly = (_int)neigh;
		}
	}
	
	return false;
}

vector3 CNavMeshAgent::ProjectToPolyPlane(const vector3& pW, const CNaviMesh::NaviPolygon& poly) const
{
	const vector3& A = poly.vertices[0];
	const vector3& B = poly.vertices[1];
	const vector3& C = poly.vertices[2];

	vector3 n = (B - A).cross(C - A);
	const _float nlen2 = n.lengthSq();
	if (nlen2 < 1e-10f) 
		return pW; // 퇴행

	n = n * (1.0f / sqrtf(nlen2));
	_float d = -n.dot(A);

	// n.y가 너무 작으면 Y를 유지
	if (fabsf(n.y) < 1e-6f)
		return pW;

	_float y = (-d - n.x * pW.x - n.z * pW.z) / n.y;
	return vector3(pW.x, y, pW.z);
}

_bool CNavMeshAgent::TryAdvanceAcrossNeighbors(const vector3& posW, _int& ioPoly, vector3& ioProj) const
{
	// 과도한 루프 방지
	const int HOP_GUARD = 16;
	_int cur = ioPoly;
	vector3 proj = {};

	for (_int hop = 0; hop < HOP_GUARD; ++hop)
	{
		_int next = -1;
		if (IsInsideOrGetNeighbor(posW, cur, next, proj, nullptr)) {
			ioPoly = cur;
			ioProj = proj;
			return true;
		}
		if (next < 0) break; // 더 이상 이웃 없음 → 실패
		cur = next;
	}
	return false;
}

void CNavMeshAgent::RollbackToPrevPosition()
{
	auto* tr = m_pGameObject->Get_Transform();
	tr->Set_Position(tr->Get_PrevPosition());
}
