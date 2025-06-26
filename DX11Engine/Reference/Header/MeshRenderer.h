#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshRenderer final : public CComponent
{
protected:
	explicit CMeshRenderer();
	~CMeshRenderer();

public:
	static CMeshRenderer* Create();

public:
	HRESULT Initialize() override;
	void OnPreCull() override;
	void OnPreRender() override;
	void Render_Editor() override;
	void Render() override;
	void OnPostRender() override;

	void OnDestroy() override;

	void Render_WithCamera(CCamera* _cam);

public:
	CMeshFilter* Get_MeshFilter();
	void Set_Material(CMaterial* pMaterial);

private:
	CMeshFilter* m_pMeshFilter;
	CMaterial* m_pMaterial;
};

NS_END

