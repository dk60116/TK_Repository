#pragma once

#include "MeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSkinnedMeshBuffer : public CMeshBuffer
{
	friend class CResources;
	friend class CSkinnedMeshRenderer;

private:
	explicit CSkinnedMeshBuffer();
	~CSkinnedMeshBuffer();

private:
	static CSkinnedMeshBuffer* Create(const wstring& _filePath);

protected:
	HRESULT Initialize(const wstring& _name, const wstring& _filePath, void* _desc) override;
	void Render();
	void OnDestroy() override;

private:
	void FillBoneWeightsAndIndices(const aiMesh* mesh, vector<VertexSkinnedBuffer>& vertices);
	const _float4x4& Get_BoneOffsetMatrix(const _uint _index);

private:
	Assimp::Importer* m_pImporter;
	const aiScene* m_pAssimpScene;
	vector<wstring> m_vBoneNames;
	vector<_float4x4> m_vBoneOffsetMatrices;
};

NS_END

