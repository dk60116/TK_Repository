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

public:
	HRESULT Initialize(const wstring& _name, wstring _filePath, void* _desc);
	void Render();
	void OnDestroy();

private:
	void FillBoneWeightsAndIndices(const aiMesh* mesh, std::vector<VertexSkinnedBuffer>& vertices);

private:
	Assimp::Importer* m_pImporter;
	const aiScene* m_pAssimpScene;
	vector<wstring> m_vBoneNames;
	vector<XMMATRIX> m_vBoneOffsetMatrices;
};

NS_END

