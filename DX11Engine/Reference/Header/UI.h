#pragma once

#include "Component.h"

#include "UIManager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CComponent
{
protected:
	CUI();
	~CUI();

public:
	HRESULT Initialize() override;
	void OnDestroy();

public:
	void Set_Mesh(CMeshBuffer* _mesh);
	void Set_Material(CMaterial* _material);

protected:
	CRectTransform* m_pRectTransform;
	class CCanvas* m_pCanvas;
	CMeshBuffer* m_pRectMesh;
	CMaterial* m_pMaterial;

protected:
	CMeshBuffer* m_pRectGizmoMesh;
	CMaterial* m_pLineMat;
};

NS_END

