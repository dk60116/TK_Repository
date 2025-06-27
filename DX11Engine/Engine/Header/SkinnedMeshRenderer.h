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

	void  CreateSkinnedMeshBuffer(const string& _filePath, const _float _scaleFactor = 1.f);

protected:
	void Render_WithCamera(CCamera* _cam) override;

public:
	void SetMesh(CMeshBuffer* _Mesh);
	void SetBones(const vector<CTransform*>& _bones);
	void SetRootBone(CTransform* _root);

private:
	CSkinnedMeshBuffer* m_pMeshBuffer;
	vector<CTransform*> m_vBones;
	CTransform* m_pRootBone;

	ID3D11Buffer* m_pBoneMatrixBuffer = nullptr;
};

NS_END

