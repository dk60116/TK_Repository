#include "epch.h"
#include "NaviMesh.h"

using namespace EngineAI;

CNaviMesh::CNaviMesh()
{
}

CNaviMesh::~CNaviMesh()
{
}

CNaviMesh* CNaviMesh::Create()
{
    return new CNaviMesh();
}

CNaviMesh::NaviMeshBufferInitiaizeInfo CNaviMesh::BuildFromMesh(vector<CMeshBuffer*> _sourceMeshes, NavBakeOptions _bakeOption)
{
    NaviMeshBufferInitiaizeInfo info{};               // 반환 값 기본 NULL

    if (_sourceMeshes.empty())
    {
        CDebug::LogError("BuildFromMesh failed: source meshs is empty.");
        return info;
    }

    using VERT = VertexNormalColorBuffer;

    vector<VERT>  vertsMerged;
    vector<_uint> idxMerged;
    vertsMerged.reserve(1024);           
    idxMerged.reserve(2048);

    _uint vertOffset = 0;

    for (const CMeshBuffer* m : _sourceMeshes)
    {
        if (!m)
            continue;

        auto verts = m->Get_VertexBuffer();
        auto indices = m->Get_IndexBuffer();
        if (verts.empty() || indices.empty()) 
            continue;

        for (const auto& src : verts)
        {
            VertexNormalColorBuffer dst;
            dst.position = src.position;
            dst.normal = src.normal;
            dst.color = { 1, 1, 1, 1 }; 

            vertsMerged.push_back(dst);
        }

        for (_uint idx : indices)
            idxMerged.push_back(idx + vertOffset);

        vertOffset += static_cast<_uint>(verts.size());
    }

    if (vertsMerged.empty() || idxMerged.empty())
    {
        CDebug::LogError("BuildFromMesh failed: merge output is empty");
        return info;
    }

    vector<array<_uint, 3>> walkables;
    BuildWalkableTriangleList(vertsMerged, idxMerged,
        _bakeOption.walkableSlopeDeg, walkables);

    if (walkables.empty())
    {
        CDebug::LogError("BuildFromMesh: Walkable 삼각형이 없습니다");
        return info;
    }

    unordered_map<EdgeKey, _uint, EdgeKeyHash> edgeOwner;
    vector<Poly> polys;
    polys.reserve(walkables.size());

    for (const auto& tri : walkables)
    {
        Poly p;
        p.index = static_cast<_uint>(polys.size());
        p.verts = { tri[0], tri[1], tri[2] };
        p.neighs.resize(3, UINT_MAX);

        const _vector c =
            XMVectorScale(
                XMVectorAdd(
                    XMVectorAdd(
                        XMLoadFloat3(&vertsMerged[tri[0]].position),
                        XMLoadFloat3(&vertsMerged[tri[1]].position)
                    ),
                    XMLoadFloat3(&vertsMerged[tri[2]].position)
                ),
                1.f / 3.f
            );

        XMStoreFloat3(reinterpret_cast<_float3*>(&p.center), c);

        for (int e = 0; e < 3; ++e)
        {
            EdgeKey k = MakeEdge(p.verts[e], p.verts[(e + 1) % 3]);
            auto it = edgeOwner.find(k);

            if (it == edgeOwner.end())
                edgeOwner[k] = p.index;                    // 최초 등록
            else
            {                                             // 공유 에지 → 인접
                const _uint other = it->second;
                p.neighs[e] = other;
                // other 폴리의 대응 edge 찾아 채우기
                auto& neigh = polys[other].neighs;
                for (auto& idx : neigh)
                    if (idx == UINT_MAX) { idx = p.index; break; }
            }
        }
        polys.push_back(move(p));
    }

    info.polygons.clear();
    info.polygons.reserve(polys.size());

    for (const Poly& p : polys)
    {
        NaviPolygon np;
        np.index = p.index;
        np.neighbors.assign(p.neighs.begin(), p.neighs.end());
        for (_uint vi : p.verts)
            np.vertices.push_back(reinterpret_cast<const vector3&>(vertsMerged[vi].position));
        info.polygons.push_back(std::move(np));
    }

    info.meshName = L"NaviMesh_Merged";

    info.buffer.assign(reinterpret_cast<const uint8_t*>(vertsMerged.data()),
        reinterpret_cast<const uint8_t*>(vertsMerged.data()) +
        vertsMerged.size() * sizeof(VERT));

    info.indices.assign(idxMerged.begin(), idxMerged.end());

    info.desc.vertexSize = sizeof(VERT);
    info.desc.vertextCount = static_cast<_uint>(vertsMerged.size());
    info.desc.indexCount = static_cast<_uint>(idxMerged.size());
    info.desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    CDebug::Log(L"BuildFromMesh Complete: Poly " + to_wstring(polys.size()) +
        L", vertex " + to_wstring(info.desc.vertextCount) +
        L", index " + to_wstring(info.desc.indexCount));

    return info;
}

void CNaviMesh::Render_Editor()
{
}

void CNaviMesh::BuildWalkableTriangleList(const vector<VertexNormalColorBuffer>& _verts, const vector<_uint>& _indices, _float _maxSlopeDeg, vector<array<_uint, 3>>& _outWalkables)
{
    _outWalkables.clear();
    if (_verts.empty() || _indices.empty() || (_indices.size() % 3))
        return;

    const _vector UP = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    const float    cosLimit = cosf(XMConvertToRadians(_maxSlopeDeg));

    for (size_t i = 0; i < _indices.size(); i += 3)
    {
        _uint i0 = _indices[i + 0];
        _uint i1 = _indices[i + 1];
        _uint i2 = _indices[i + 2];

        const _vector p0 = XMLoadFloat3(&_verts[i0].position);
        const _vector p1 = XMLoadFloat3(&_verts[i1].position);
        const _vector p2 = XMLoadFloat3(&_verts[i2].position);

        _vector n = XMVector3Cross(XMVectorSubtract(p1, p0),
            XMVectorSubtract(p2, p0));
        if (XMVector3Equal(n, XMVectorZero())) 
            continue;
        n = XMVector3Normalize(n);

        const _float dotUp = fabsf(XMVectorGetX(XMVector3Dot(n, UP)));
        if (dotUp >= cosLimit)
            _outWalkables.push_back({ i0, i1, i2 });
    }
}

CNaviMesh::EdgeKey CNaviMesh::MakeEdge(_uint _a, _uint _b)
{
    return (_a < _b) ? CNaviMesh::EdgeKey{ _a, _b } : CNaviMesh::EdgeKey{ _b, _a };
}
