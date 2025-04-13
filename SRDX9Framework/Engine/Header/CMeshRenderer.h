#pragma once

#include "CComponent.h"
#include "Engine_Define.h"
#include "CMeshFilter.h"
#include "CMaterial.h"

BEGIN(Engine)

class ENGINE_DLL CMeshRenderer 
    : public CComponent
{
public:
    CMeshRenderer();
    ~CMeshRenderer();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void Render() override;
    void OnDestroy() override;

private:
    CMeshFilter* m_pMeshFilter;
    CMaterial* m_pMaterial;
};

END

