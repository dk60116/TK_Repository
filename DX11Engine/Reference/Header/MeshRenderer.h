#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL MeshRenderer final : public CComponent
{
protected:
	explicit MeshRenderer();
	~MeshRenderer();

public:
	static MeshRenderer* Create();

public:
	void Initialize() override;
	void OnPreCull() override;
	void OnPreRender() override;
	void Render() override;
	void OnPostRender() override;

public:
	void Set_MeshFilter(class CMeshFilter* pFilter);
	void Set_Material(class CMaterial* pMaterial);

private:
	class CMeshFilter* m_pMeshFilter;
};

NS_END

