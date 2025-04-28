#pragma once

#include "CComponent.h"
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMeshFilter final
    : public CComponent
{
private:
    explicit CMeshFilter();
    ~CMeshFilter();

public:
    static CMeshFilter* Create();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void Render() override;
    void OnDestroy() override;

public:
    CMesh* getMesh() const { return m_pMesh; }
    void SetMesh(CMesh::MeshType _type);

private:
    CMesh* m_pMesh;
};

END
