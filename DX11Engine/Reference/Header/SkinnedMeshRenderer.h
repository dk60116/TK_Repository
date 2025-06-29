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
	CTransform* CreateBoneHierarchy(const aiNode* _node, CTransform* _parent);
	void CollectBonesRecursive(CTransform* _node, const vector<string>& _boneNames);
	void EnsureBoneCB(const UINT _boneCount);

protected:
	void Render_WithCamera(CCamera* _cam) override;

public:
	void SetMesh(CMeshBuffer* _Mesh);
	void SetBones(const vector<CTransform*>& _bones);
	void SetRootBone(CTransform* _root);
	CTransform* FindBone(const wstring& _boneName);

private:
	CSkinnedMeshBuffer* m_pMeshBuffer;
	vector<CTransform*> m_vBones;
	CTransform* m_pRootBone;
	UINT m_iCBSize;

	ID3D11Buffer* m_pBoneMatrixBuffer = nullptr;
};

NS_END

