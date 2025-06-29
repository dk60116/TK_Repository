#pragma once

#include "Object.h"

class ENGINE_DLL CSkinnedMeshBuffer : public UObject
{
private:
	struct VertexSkinned
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		XMFLOAT3 tangent;

		UINT boneIndices[4];
		float boneWeights[4];
	};

private:
	explicit CSkinnedMeshBuffer();
	~CSkinnedMeshBuffer();

public:
	static CSkinnedMeshBuffer* Create();

	HRESULT Initialize(const string& _filePath, float _scaleFactor = 1.0f);
	void Render();
	void OnDestroy();

public:
	ID3D11Buffer* Get_VertexBuffer() const;
	ID3D11Buffer* Get_IndexBuffer() const;
	UINT Get_IndexCount() const;

	const _matrix& Get_BindPoseMatricesInverse(const _int _index);
	const vector<string>& Get_BoneNames() const;

private:
	void EnsureBoneCB(const UINT _boneCount);

private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	UINT m_iIndexCount;

	vector<string> m_vBoneNames;

	vector<_matrix> m_vBindPoseInverse;

	vector<VertexSkinned> m_vVertices;
};

