#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshBuffer : public CEngineResource
{
	friend class CResources;

public:
	typedef struct MeshBufferDescription
	{
		_bool useDeviceTopology = false;
		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		_uint vertexSize = 0;
		_uint vertextCount = 0;
		_uint indexCount = 0;
	}MESHBUFFERDESC;

	struct MeshBufferInitiaizeInfo
	{
		vector<uint8_t> buffer = {};
		vector<UINT> indices = {};
		MESHBUFFERDESC desc = {};
	};

	typedef struct TerainMeshBufferDesctiption
	{
		_bool heightMapBase = false;
		_uint landscape = 100;
		_uint portrait = 100;
		_float size = 0.5f;
		class CTexture* heightMap = nullptr;
	}TERRAINBUFFERDESC;

protected:
	explicit CMeshBuffer();
	~CMeshBuffer();

private:
	static CMeshBuffer* Create();

protected:
	HRESULT Initialize(const wstring& _name, const wstring& _filePath, void* _desc) override;
	void OnDestroy();

public:
	HRESULT Initailize_Custom(MeshBufferInitiaizeInfo _info, void* _desc);

public:
	static wstring FindMeshName(const aiScene* scene, _uint meshIndex, aiNode* node = nullptr);

private:
	MeshBufferInitiaizeInfo CreateLine();
	MeshBufferInitiaizeInfo CreateLineRect();
	MeshBufferInitiaizeInfo CreateRect();
	MeshBufferInitiaizeInfo CreateCube();
	MeshBufferInitiaizeInfo CreateSphere();
	MeshBufferInitiaizeInfo CreatePlane();
	MeshBufferInitiaizeInfo CreateQuad();
	MeshBufferInitiaizeInfo CreateCylinder();
	MeshBufferInitiaizeInfo CreateTriangle();
	MeshBufferInitiaizeInfo CreateTerrain(_uint _sizeX, _uint _sizeZ, const _float _size);

	static MeshBufferInitiaizeInfo CreateObjectMesh(const aiScene* _aiScene, const _uint _index = 0, const _float _scaleFactor = 1.f);

public:
	void Render();

public:
	virtual void Set_Scalefactor(const _float _value);
	ID3D11Buffer* Get_VertexBuffer() const;
	ID3D11Buffer* Get_IndexBuffer() const;
	const MESHBUFFERDESC& Get_Info();

protected:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	MESHBUFFERDESC m_sInfo;

private:
	void* m_pVertexSysMem;
	void* m_pIndexSysMem ;
};

NS_END

