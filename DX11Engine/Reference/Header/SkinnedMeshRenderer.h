#pragma once

#include "Renderer.h"
#include "SkinnedMeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSkinnedMeshRenderer : public CRenderer
{
	friend class CGameObject;

private:
	CSkinnedMeshRenderer();
	~CSkinnedMeshRenderer();

private:
	static CSkinnedMeshRenderer* Create();

public:
	HRESULT Initialize() override;
	void OnPreCull() override;
	void OnPreRender() override;
	void Render_Editor() override;
	void Render() override;
	void OnPostRender() override;
	void OnDestroy()override;

protected:
	void Render_WithCamera(CCamera* _cam) override;

public:
	void SetMesh(CMeshBuffer* _Mesh);

private:
	CSkinnedMeshBuffer* m_pMeshBuffer;
	vector<CTransform*> m_vBones;
	CTransform* m_pRootBone;
	UINT m_iCBSize;

	ID3D11Buffer* m_pBoneMatrixBuffer = nullptr;
};

NS_END

