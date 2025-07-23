#pragma once

#include "MeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSkinnedMeshBuffer : public CMeshBuffer
{
	friend class CResources;
	friend class CSkinnedMeshRenderer;

public:
	struct SkinnedBufferInitiaizeInfo
	{
		wstring meshName = L"";
		vector<uint8_t> buffer = {};
		vector<_uint> indices = {};
		MESHBUFFERDESC desc = {};
		wstring diffuseMapPath = L"";
		vector<wstring> boneNames;
		vector<_float4x4> boneOffsetMatrices;
	};

private:
	explicit CSkinnedMeshBuffer();
	~CSkinnedMeshBuffer();

private:
	static CSkinnedMeshBuffer* Create();

protected:
	HRESULT Initialize(const wstring& _name, const wstring& _filePath, void* _desc) override;
	void Render();
	void OnDestroy() override;

public:
	static void FillBoneWeights(VertexSkinnedBuffer& _targetBuffer, const _uint _index, const _float _weight);

private:
	static void FillBoneWeightsAndIndices(const aiMesh* mesh, vector<VertexSkinnedBuffer>& vertices);
	const _float4x4& Get_BoneOffsetMatrix(const _uint _index);

private:
	Assimp::Importer* m_pImporter;
	const aiScene* m_pAssimpScene;
	vector<wstring> m_vBoneNames;
	vector<_float4x4> m_vBoneOffsetMatrices;
};

NS_END

