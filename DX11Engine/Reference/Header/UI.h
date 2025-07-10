#pragma once

#include "Component.h"

#include "UIManager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CComponent
{
	friend class CRectTransform;

protected:
	CUI();
	~CUI();

public:
	HRESULT Initialize() override;
	void OnDestroy();

public:
	void Set_Mesh(CMeshBuffer* _mesh);
	void Set_Material(CMaterial* _material);

public:
	void Bind_Matrix(const _fmatrix _view, const _cmatrix _projection);

private:
	void Set_Canvas(class CCanvas* _canvas);

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

