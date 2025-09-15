#pragma once

#include "Renderer.h"
#include "MeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshRenderer final : public CRenderer
{
	friend class CGameObject;

protected:
	explicit CMeshRenderer();
	~CMeshRenderer();

private:
	static CMeshRenderer* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void OnPreCull() override;
	void OnPreRender() override;
	void Render_Editor() override;
	void Render() override;
	void Render_Gizmo() override;

	void OnDestroy() override;

public:
	void Render_WithCamera(CCamera* _cam) override;
	void Render_Outline(CCamera* _cam) override;

public:
	CMeshFilter* Get_MeshFilter();
	CMeshBuffer* Get_MeshBuffer() override;

	void Set_ScaleFactor(const _float _value);

private:
	CMeshFilter* m_pMeshFilter;
};

NS_END

