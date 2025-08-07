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

HRESULT CNaviMesh::Initailize_Custom(const NaviMeshBufferInitiaizeInfo _info, void* _desc)
{
    if (FAILED(__super::Initailize_Custom(_info, _desc)))
        return E_FAIL;

    m_vPolygons = _info.polygons;

    return S_OK;
}

CNaviMesh::NaviMeshBufferInitiaizeInfo CNaviMesh::BuildFromMesh(vector<CGameObject*> _sourceObjs, NavBakeOptions _bakeOption)
{
    struct Src { CMeshBuffer* buf; CGameObject* obj; };
    vector<Src> sources;
    for (auto* go : _sourceObjs) {
        if (!go) continue;
        if (auto* rnd = go->GetComponent<CMeshRenderer>())
            sources.push_back({ rnd->Get_MeshBuffer(), go });
    }

    NaviMeshBufferInitiaizeInfo info{};
    if (sources.empty()) {
        CDebug::LogError("BuildFromMesh failed: no source meshes");
        return info;
    }

    /*──────────────────────────────────────────────────────────
      1) 병합 정점·인덱스 (월드 좌표 변환 포함)
    ──────────────────────────────────────────────────────────*/
    using VTX = VertexNormalColorBuffer;
    vector<VTX>  vertsMerged;
    vector<_uint> idxMerged;
    _uint vertOffset = 0;

    for (const auto& s : sources) 
    {
        if (!s.buf)
            continue;

        const auto& vBuf = s.buf->Get_VertexBuffer();
        const auto& iBuf = s.buf->Get_IndexBuffer();
        if (vBuf.empty() || iBuf.empty()) continue;

        const _matrix world = s.obj->Get_Transform()->Get_WorldMatrix();
        const _matrix nMat = XMMatrixTranspose(XMMatrixInverse(nullptr, world));

        for (const auto& vin : vBuf) {
            VTX vout;
            // position
            XMStoreFloat3(&vout.position,
                XMVector3TransformCoord(XMLoadFloat3(&vin.position), world));
            // normal
            XMStoreFloat3(&vout.normal,
                XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&vin.normal), nMat)));
            vout.color = { 1,1,1,1 };
            vertsMerged.push_back(vout);
        }
        for (_uint idx : iBuf)
            idxMerged.push_back(idx + vertOffset);

        vertOffset += static_cast<_uint>(vBuf.size());
    }

    if (vertsMerged.empty() || idxMerged.empty()) {
        CDebug::LogError("BuildFromMesh failed: merged data empty");
        return info;
    }

    /*──────────────────────────────────────────────────────────
      2) Walkable 삼각형 필터
    ──────────────────────────────────────────────────────────*/
    vector<array<_uint, 3>> walkables;
    BuildWalkableTriangleList
    (
        vertsMerged, idxMerged,
        _bakeOption.walkableSlopeDeg,
        _bakeOption.walkableMaxHeight,
        walkables
    );

    if (walkables.empty()) 
    {
        CDebug::LogError("BuildFromMesh failure: no walkable tris");
        return info;
    }

    /*──────────────────────────────────────────────────────────
      3) EdgeKey → 폴리그래프
    ──────────────────────────────────────────────────────────*/
    unordered_map<EdgeKey, _uint, EdgeKeyHash> edgeOwner;
    vector<Poly> polys; polys.reserve(walkables.size());

    for (auto& tri : walkables) {
        Poly p;
        p.index = (_uint)polys.size();
        p.verts = { tri[0],tri[1],tri[2] };
        p.neighs.resize(3, UINT_MAX);

        // center
        _vector c = XMVectorScale(
            XMVectorAdd(XMVectorAdd(
                XMLoadFloat3(&vertsMerged[tri[0]].position),
                XMLoadFloat3(&vertsMerged[tri[1]].position)),
                XMLoadFloat3(&vertsMerged[tri[2]].position)),
            1.f / 3.f);
        XMStoreFloat3(reinterpret_cast<_float3*>(&p.center), c);

        // edge map
        for (int e = 0; e < 3; ++e) {
            EdgeKey k = MakeEdge(p.verts[e], p.verts[(e + 1) % 3]);
            auto it = edgeOwner.find(k);
            if (it == edgeOwner.end()) edgeOwner[k] = p.index;
            else {
                const _uint o = it->second;
                p.neighs[e] = o;
                auto& neigh = polys[o].neighs;
                for (auto& nx : neigh) if (nx == UINT_MAX) { nx = p.index; break; }
            }
        }
        polys.push_back(move(p));
    }

    /*──────────────────────────────────────────────────────────
      4) Walkable 전용 버퍼 재구성 (정점 압축 + 인덱스 리맵)
    ──────────────────────────────────────────────────────────*/
    vector<VTX>   navVerts;  navVerts.reserve(walkables.size() * 3);
    vector<_uint> navIdx;    navIdx.reserve(walkables.size() * 3);
    unordered_map<_uint, _uint> remap;

    auto Remap = [&](_uint old)->_uint {
        auto [it, ins] = remap.try_emplace(old, (_uint)navVerts.size());
        if (ins) navVerts.push_back(vertsMerged[old]);
        return it->second;
        };

    for (auto& tri : walkables) {
        navIdx.push_back(Remap(tri[0]));
        navIdx.push_back(Remap(tri[1]));
        navIdx.push_back(Remap(tri[2]));
    }

    /*──────────────────────────────────────────────────────────
      5) info 채우기 & 반환
    ──────────────────────────────────────────────────────────*/
    info.meshName = L"NaviMesh_Walkable";

    info.buffer.assign(
        reinterpret_cast<const uint8_t*>(navVerts.data()),
        reinterpret_cast<const uint8_t*>(navVerts.data()) + sizeof(VTX) * navVerts.size());

    info.indices.swap(navIdx);

    info.desc.vertexSize = sizeof(VTX);
    info.desc.vertextCount = (_uint)navVerts.size();
    info.desc.indexCount = (_uint)info.indices.size();
    info.desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    /* 폴리 리스트 저장 (info.polygons) */
    info.polygons.clear(); info.polygons.reserve(polys.size());
    for (const auto& p : polys) {
        NaviPolygon np;
        np.index = p.index;
        np.neighbors = p.neighs;
        for (_uint vi : p.verts) {
            const auto& pos = vertsMerged[vi].position;
            np.vertices.emplace_back(pos.x, pos.y, pos.z);
        }
        info.polygons.push_back(move(np));
    }

    CDebug::Log(L"NavMesh Bake Poly:" + to_wstring(polys.size()) +
        L", Vert:" + to_wstring(info.desc.vertextCount));

    return info;
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

void CNaviMesh::BuildWalkableTriangleList(const vector<VertexNormalColorBuffer>& _verts, const vector<_uint>& _indices, const _float _maxSlopeDeg, const _float _maxStepHeight, vector<array<_uint, 3>>& _outWalkables)
{
    _outWalkables.clear();
    if (_verts.empty() || _indices.empty() || _indices.size() % 3) return;

    const _vector UP = XMVectorSet(0, 1, 0, 0);
    const float   cosLimit = cosf(XMConvertToRadians(_maxSlopeDeg));
    const float   EPS = 1e-4f;

    for (size_t i = 0; i < _indices.size(); i += 3)
    {
        _uint i0 = _indices[i], i1 = _indices[i + 1], i2 = _indices[i + 2];

        const _vector p0 = XMLoadFloat3(&_verts[i0].position);
        const _vector p1 = XMLoadFloat3(&_verts[i1].position);
        const _vector p2 = XMLoadFloat3(&_verts[i2].position);

        /* 1. 노멀·천장 필터 */
        _vector n = XMVector3Cross(XMVectorSubtract(p1, p0), XMVectorSubtract(p2, p0));
        if (XMVectorGetX(XMVector3LengthSq(n)) < 1e-8f)        continue;
        n = XMVector3Normalize(n);

        const float dotUp = XMVectorGetX(XMVector3Dot(n, UP));
        if (dotUp <= 0.f)                                      continue; // 천장

        /* 2. 기울기 계산 */
        bool passSlope = (dotUp + EPS >= cosLimit);  // 경사 OK?

        /* 3. ΔY 계산 (작은 턱 허용) */
        const _float y0 = _verts[i0].position.y,
            y1 = _verts[i1].position.y,
            y2 = _verts[i2].position.y;
        const _float deltaY = max(max(y0, y1), y2) - min(min(y0, y1), y2);
        _bool passStep = (deltaY <= _maxStepHeight);

        if (passSlope || passStep)
            _outWalkables.push_back({ i0,i1,i2 });
    }
}

CNaviMesh::EdgeKey CNaviMesh::MakeEdge(_uint _a, _uint _b)
{
    return (_a < _b) ? CNaviMesh::EdgeKey{ _a, _b } : CNaviMesh::EdgeKey{ _b, _a };
}
