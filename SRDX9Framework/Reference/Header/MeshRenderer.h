#pragma once

#include "Component.h"
#include "Engine_Define.h"
#include "MeshFilter.h"
#include "Material.h"
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CMeshRenderer final
    : public CComponent
{
private:
    explicit CMeshRenderer();
    ~CMeshRenderer();

public:
    static CMeshRenderer* Create();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void RenderEditor() override;
    void Render() override;
    void OnDestroy() override;

private:
    void Render_Final(CCamera* _camera, _bool _editor);

public:
    CMeshFilter* getMeshFilter() const { return m_pMeshFilter; }
    void SetMeshFilterType(CMesh::MeshType _type);
    void SetMaterial(CMaterial* _mat) { m_pMaterial = _mat; }

private:
    CMeshFilter* m_pMeshFilter;
    CMaterial* m_pMaterial;

    BEGIN_SERIALIZEFIELD
    END_SERIALIZEFIELD
};

END

