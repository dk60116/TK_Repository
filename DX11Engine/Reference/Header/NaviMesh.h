#pragma once

#include "MeshBuffer.h"

NS_BEGIN(Engine)

NS_BEGIN(EngineAI)

class ENGINE_DLL CNaviMesh final : public CMeshBuffer
{
	friend class CResources;

public:
	struct NavBakeOptions
	{
		_float walkableSlopeDeg = 45.f;   // 최대 기울기
		_float walkableMaxHeight = 0.2f;
		_float agentRadius = 0.3f;   // 반경
		_float agentHeight = 1.8f;   // 높이
		_float cellSize = 0.2f;   // 샘플 그리드 간격
		_float cellHeight = 0.2f;
		_bool  keepTriangles = false;
	};

	struct EdgeKey
	{
		_uint a, b;	// 정점 인덱스(작은 번호→큰 번호 정렬)
		
		bool operator==(const EdgeKey& other) const
		{
			return a == other.a && b == other.b; 
		}
	};

	struct EdgeKeyHash
	{
		size_t operator()(const EdgeKey& k) const
		{
			return (size_t)k.a * 73856093u ^ (size_t)k.b * 19349669u;
		}
	};

	struct Poly  
	{
		_uint index;              // NaviPolygon.index 와 동일
		vector<_uint> verts;      // 정점 인덱스
		vector<_uint> neighs;     // 인접 Poly index
		_vector center;           // 중앙값(휴리스틱용)
	};

	struct NaviPolygon
	{
		_uint index;
		vector<_uint> neighbors;
		vector<vector3> vertices;
	};

	struct PathNode
	{
		_uint polygonIndex;
		float gCost, hCost;
		PathNode* parent;
	};

	struct NaviMeshBufferInitiaizeInfo : public MeshBufferInitiaizeInfo
	{
		vector<NaviPolygon> polygons;
	};

private:
	CNaviMesh();
	~CNaviMesh();

private:
	static CNaviMesh* Create();

public:
	static NaviMeshBufferInitiaizeInfo BuildFromMesh(vector<CGameObject*> _sourceObjs, NavBakeOptions _bakeOption);
	_bool FindPath(const vector3& _start, const vector3& _end, vector<vector3>& _outPath);
	_int FindContainingPolygon(const vector3& _position);

	void Render_Editor();
	
private:
	static void BuildWalkableTriangleList(
		const vector<VertexNormalColorBuffer>& _verts,
		const vector<_uint>& _indices,
		const _float _maxSlopeDeg,
		const _float _maxStepHeight,
		vector<array<_uint, 3>>& _outWalkables);

	static CNaviMesh::EdgeKey MakeEdge(_uint _a, _uint _b);
};

NS_END

NS_END

