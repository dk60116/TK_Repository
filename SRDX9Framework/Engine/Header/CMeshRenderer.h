#pragma once

#include "CComponent.h"
#include "Engine_Define.h"
#include "CMeshFilter.h"
#include "CMaterial.h"

BEGIN(Engine)

class ENGINE_DLL CMeshRenderer final
    : public CComponent
{
public:
    explicit CMeshRenderer();
    ~CMeshRenderer();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void Render() override;
    void OnDestroy() override;

public:
    CMeshFilter* getMeshFilter() const { return m_pMeshFilter; }
    void SetMeshFilterType(CMesh::MeshType _type);
    void SetMaterial(CMaterial* _mat) { m_pMaterial = _mat; }

private:
    CMeshFilter* m_pMeshFilter;
    CMaterial* m_pMaterial;
};

END

