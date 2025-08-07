#include "epch.h"
#include "NaviMesh.h"

using namespace EngineAI;

CNaviMesh::CNaviMesh()
    : m_vPolygons({})
{
}

CNaviMesh::~CNaviMesh()
{
}

CNaviMesh* CNaviMesh::Create()
{
    return new CNaviMesh();
}

HRESULT CNaviMesh::Initialize_Custom(const NaviMeshBufferInitiaizeInfo _info, void* _desc)
{
    if (FAILED(__super::Initialize_Custom(_info, _desc)))
        return E_FAIL;

    m_vPolygons = _info.polygons;

    return S_OK;
}

vector<CNaviMesh::NaviMeshBufferInitiaizeInfo> CNaviMesh::BuildFromMesh(vector<CGameObject*> _sourceObjs, NavBakeOptions _bakeOption)
{
    using VTX = VertexTexNormalTangentBuffer;
    struct Src { CMeshBuffer* buf; CGameObject* obj; };

    /*──────────────────────────────────────── 0) MeshRenderer 수집 */
    vector<Src> sources;
    for (auto* go : _sourceObjs)
        if (go)
            if (auto* mr = go->GetComponent<CMeshRenderer>())
                sources.push_back({ mr->Get_MeshBuffer(), go });

    if (sources.empty())
    {
        CDebug::LogError("BuildFromMesh failed: no source meshes");
        return {};
    }

    /*──────────────────────────────────────── 1) 정점 병합 */
    struct PosKey {
        int64_t x, y, z;  bool operator==(const PosKey& o) const
        {
            return x == o.x && y == o.y && z == o.z;
        }
    };
    struct PosHash {
        size_t operator()(const PosKey& k) const
        {
            return size_t(k.x) * 73856093u ^ size_t(k.y) * 19349669u ^ size_t(k.z) * 83492791u;
        }
    };

    constexpr float POS_EPS = 1e-3f;
    auto quant = [](_float v)->int64_t { return llround(v / POS_EPS); };

    vector<VTX>                 vertsMerged;
    unordered_map<PosKey, _uint, PosHash> posTable;
    auto AddVertex = [&](const VTX& v)->_uint
        {
            PosKey k{ quant(v.position.x), quant(v.position.y), quant(v.position.z) };
            auto [it, ins] = posTable.try_emplace(k, (_uint)vertsMerged.size());
            if (ins) vertsMerged.push_back(v);
            return it->second;
        };

    vector<_uint> idxMerged; idxMerged.reserve(4096);

    /*──────────────────────────────────────── 2) 월드 변환 → 병합 */
    for (auto& s : sources)
    {
        if (!s.buf) continue;
        const auto& vBuf = s.buf->Get_VertexBuffer();
        const auto& iBuf = s.buf->Get_IndexBuffer();
        if (vBuf.empty() || iBuf.empty()) continue;

        _matrix W = s.obj->Get_Transform()->Get_WorldMatrix();
        _matrix nM = XMMatrixTranspose(XMMatrixInverse(nullptr, W));

        vector<_uint> l2g(vBuf.size());
        for (size_t i = 0; i < vBuf.size(); ++i)
        {
            VTX v = vBuf[i];
            XMStoreFloat3(&v.position, XMVector3TransformCoord(vector3::ToXM(v.position), W));
            XMStoreFloat3(&v.normal, XMVector3Normalize(XMVector3TransformNormal(vector3::ToXM(v.normal), nM)));
            l2g[i] = AddVertex(v);
        }
        for (auto idx : iBuf) idxMerged.push_back(l2g[idx]);
    }

    if (vertsMerged.empty() || idxMerged.empty())
    {
        CDebug::LogError("BuildFromMesh failed: merged data empty");
        return {};
    }

    /*──────────────────────────────────────── 3) 앞·뒤 중복 삼각형 제거 */
    struct TriKey {
        array<_uint, 3> v;
        void Sort() { std::sort(v.begin(), v.end()); }
        bool operator==(const TriKey& o) const { return v == o.v; }
    };
    struct TriHash {
        size_t operator()(const TriKey& k) const
        {
            return size_t(k.v[0]) * 73856093u ^ size_t(k.v[1]) * 19349669u ^ size_t(k.v[2]) * 83492791u;
        }
    };
    struct TriInfo { _vector n; int vecIdx; };

    unordered_map<TriKey, TriInfo, TriHash> triTable;
    vector<array<_uint, 3>> triVec; triVec.reserve(idxMerged.size() / 3);

    for (size_t i = 0; i < idxMerged.size(); i += 3)
    {
        _uint i0 = idxMerged[i], i1 = idxMerged[i + 1], i2 = idxMerged[i + 2];
        TriKey key{ {i0,i1,i2} }; key.Sort();

        _vector p0 = vector3::ToXM(vertsMerged[i0].position);
        _vector p1 = vector3::ToXM(vertsMerged[i1].position);
        _vector p2 = vector3::ToXM(vertsMerged[i2].position);

        _vector n = XMVector3Normalize(
            XMVector3Cross(XMVectorSubtract(p1, p0), XMVectorSubtract(p2, p0)));

        auto it = triTable.find(key);
        if (it == triTable.end())
        {
            int idx = (int)triVec.size();
            triTable[key] = { n, idx };
            triVec.push_back({ i0,i1,i2 });
        }
        else
        {
            if (XMVectorGetX(XMVector3Dot(it->second.n, n)) < -0.99f)
            {   // 정반대면 앞·뒤 함께 제거
                triVec[it->second.vecIdx] = { UINT_MAX,UINT_MAX,UINT_MAX };
                continue;
            }
            // 같은 방향 중복 → skip
        }
    }

    vector<_uint> idxFiltered;
    for (auto& t : triVec)
        if (t[0] != UINT_MAX)
        {
            idxFiltered.insert(idxFiltered.end(), { t[0], t[1], t[2] });
        }

    if (idxFiltered.empty())
    {
        CDebug::LogError("BuildFromMesh failure: all tris removed");
        return {};
    }

    /*──────────────────────────────────────── 4) Walkable / Unwalkable 분류 */
    vector<array<_uint, 3>> walkables, unwalkables;
    BuildWalkableTriangleList(
        vertsMerged, idxFiltered,
        _bakeOption.walkableSlopeDeg,
        _bakeOption.walkableMaxHeight,
        walkables,
        unwalkables);

    if (walkables.empty())
    {
        CDebug::LogError("BuildFromMesh failure: no walkable tris");
        return {};
    }

    /*──────────────────────────────────────── 5) Walkable → NaviPolygon 그래프 */
    unordered_map<EdgeKey, _uint, EdgeKeyHash> edgeOwner;
    vector<Poly> polys; polys.reserve(walkables.size());

    for (auto& tri : walkables)
    {
        Poly p;
        p.index = (_uint)polys.size();
        p.verts = { tri[0], tri[1], tri[2] };
        p.neighs.assign(3, UINT_MAX);

        _vector c = XMVectorScale(
            XMVectorAdd(XMVectorAdd(vector3::ToXM(vertsMerged[tri[0]].position),
                vector3::ToXM(vertsMerged[tri[1]].position)),
                vector3::ToXM(vertsMerged[tri[2]].position)),
            1.f / 3.f);
        XMStoreFloat3(reinterpret_cast<_float3*>(&p.center), c);

        for (int e = 0; e < 3; ++e)
        {
            EdgeKey k = MakeEdge(p.verts[e], p.verts[(e + 1) % 3]);
            auto it = edgeOwner.find(k);
            if (it == edgeOwner.end()) edgeOwner[k] = p.index;
            else
            {
                _uint o = it->second;
                p.neighs[e] = o;
                for (auto& nx : polys[o].neighs)
                    if (nx == UINT_MAX) { nx = p.index; break; }
            }
        }
        polys.emplace_back(std::move(p));
    }

    /*──────────────────────────────────────── 6) 리맵 & 버퍼 빌드 (Walkable) */
    auto BuildInfo = [&](const vector<array<_uint, 3>>& tris,
        const wstring& name,
        NaviMeshBufferInitiaizeInfo& outInfo,
        bool buildPolys)->void
        {
            vector<VTX> navVerts; navVerts.reserve(tris.size() * 3);
            vector<_uint> navIdx; navIdx.reserve(tris.size() * 3);
            unordered_map<_uint, _uint> r;
            auto R = [&](const _uint oldIdx)->_uint {
                auto [it, ins] = r.try_emplace(oldIdx, (_uint)navVerts.size());
                if (ins) navVerts.push_back(vertsMerged[oldIdx]);
                return it->second;
                };
            for (auto& tri : tris)
            {
                navIdx.push_back(R(tri[0]));
                navIdx.push_back(R(tri[1]));
                navIdx.push_back(R(tri[2]));
            }

            outInfo.meshName = name;
            outInfo.buffer.assign(reinterpret_cast<const uint8_t*>(navVerts.data()),
                reinterpret_cast<const uint8_t*>(navVerts.data()) +
                sizeof(VTX) * navVerts.size());
            outInfo.indices = std::move(navIdx);
            outInfo.desc.vertexSize = sizeof(VTX);
            outInfo.desc.vertextCount = (_uint)navVerts.size();
            outInfo.desc.indexCount = (_uint)outInfo.indices.size();
            outInfo.desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

            if (!buildPolys) return;

            outInfo.polygons.reserve(polys.size());
            for (auto& p : polys)
            {
                NaviPolygon np;
                np.index = p.index;
                np.neighbors = p.neighs;
                for (_uint vi : p.verts)
                {
                    const auto& pos = vertsMerged[vi].position;
                    np.vertices.emplace_back(pos.x, pos.y, pos.z);
                }
                outInfo.polygons.emplace_back(std::move(np));
            }
        };

    NaviMeshBufferInitiaizeInfo infoWalk, infoUnwalk;
    BuildInfo(walkables, L"NaviMesh_Walkable", infoWalk, true);
    BuildInfo(unwalkables, L"NaviMesh_Unwalkable", infoUnwalk, false);

    CDebug::Log(L"[NavBake] Walkable Poly:" + to_wstring(infoWalk.polygons.size()) +
        L", Vert:" + to_wstring(infoWalk.desc.vertextCount));
    CDebug::Log(L"[NavBake] Unwalkable Tri:" + to_wstring(unwalkables.size()));

    return { infoWalk, infoUnwalk };
}

const vector3 CNaviMesh::ProjectPointToPoly(const vector3& _p, const _uint _index) const
{
    const auto& poly = m_vPolygons[_index];
    const auto& v0 = poly.vertices[0];
    const auto& v1 = poly.vertices[1];
    const auto& v2 = poly.vertices[2];

    if (PointInTri(_p, v0, v1, v2))
        return _p;                               

    vector3 best = ClosestPointOnSegment(_p, v0, v1);
    _float   bestD = (_p - best).lengthSq();

    auto testEdge = [&](const vector3& a, const vector3& b)
        {
            vector3 q = ClosestPointOnSegment(_p, a, b);
            _float d = (_p - q).lengthSq();
            if (d < bestD) 
            { 
                best = q; 
                bestD = d;
            }
        };

    testEdge(v1, v2);
    testEdge(v2, v0);
    return best;
}

void CNaviMesh::BuildWalkableTriangleList(const vector<VertexTexNormalTangentBuffer>& _verts, const vector<_uint>& _indices, const _float _maxSlopeDeg, const _float _maxStepHeight, vector<array<_uint, 3>>& _outWalkables, vector<array<_uint, 3>>& _outWalkUnables)
{
    _outWalkables.clear();
    _outWalkUnables.clear();
    if (_verts.empty() || _indices.empty() || _indices.size() % 3)
        return;

    const _vector UP = XMVectorSet(0, 1, 0, 0);
    const _float  cosLimit = cosf(XMConvertToRadians(_maxSlopeDeg));
    const _float  EPS = 1e-4f;

    for (size_t i = 0; i < _indices.size(); i += 3)
    {
        _uint i0 = _indices[i], i1 = _indices[i + 1], i2 = _indices[i + 2];

        const _vector p0 = XMLoadFloat3(&_verts[i0].position);
        const _vector p1 = XMLoadFloat3(&_verts[i1].position);
        const _vector p2 = XMLoadFloat3(&_verts[i2].position);

        /* 1. 노멀/천장 체크 */
        _vector n = XMVector3Cross(XMVectorSubtract(p1, p0), XMVectorSubtract(p2, p0));
        if (XMVectorGetX(XMVector3LengthSq(n)) < 1e-8f) {         // 0면적 삼각형
            _outWalkUnables.push_back({ i0,i1,i2 });
            continue;
        }
        n = XMVector3Normalize(n);

        const _float dotUp = XMVectorGetX(XMVector3Dot(n, UP));
        if (dotUp <= 0.f) {                                       // 천장
            _outWalkUnables.push_back({ i0,i1,i2 });
            continue;
        }

        /* 2. 경사 & 턱높이 체크 */
        const _bool passSlope = (dotUp + EPS >= cosLimit);

        const _float y0 = _verts[i0].position.y,
            y1 = _verts[i1].position.y,
            y2 = _verts[i2].position.y;
        const _float deltaY = max(max(y0, y1), y2) - min(min(y0, y1), y2);
        const _bool  passStep = (deltaY <= _maxStepHeight);

        if (passSlope || passStep)
            _outWalkables.push_back({ i0,i1,i2 });
        else
            _outWalkUnables.push_back({ i0,i1,i2 });
    }
}

CNaviMesh::EdgeKey CNaviMesh::MakeEdge(_uint _a, _uint _b)
{
    return (_a < _b) ? CNaviMesh::EdgeKey{ _a, _b } : CNaviMesh::EdgeKey{ _b, _a };
}
