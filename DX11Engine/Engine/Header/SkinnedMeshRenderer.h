#pragma once

#include "Renderer.h"
#include "SkinnedMeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSkinnedMeshRenderer : public CRenderer
{
	friend class CGameObject;
	friend class CAnimator;

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

public:
	const _uint Get_BoneCount() const;
	const wstring Get_BoneName(const _uint _index) const;
	CTransform* Get_BoneTransform(const _uint _index) const;
	const _float4x4& Get_BoneOffsetMatrix(const _uint _index) const;

protected:
	void CreateBoneHierachy(const aiNode* _node, CTransform* _parent);
	
public:
	void Render_WithCamera(CCamera* _cam) override;
	void Render_Outline(CCamera* _cam) override;

public:
	void Set_Mesh(CSkinnedMeshBuffer* _Mesh);

private:
	CSkinnedMeshBuffer* m_pMeshBuffer;
	vector<CTransform*> m_vBones;
	CTransform* m_pRootBone;

	ID3D11Buffer* m_pBoneMatrixBuffer = nullptr;
};

NS_END

