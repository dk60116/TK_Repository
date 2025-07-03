#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshBuffer : public CEngineResource
{
	friend class CResources;

public:
	typedef struct MeshBufferDescription
	{
		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		UINT vertexSize = 0;
		UINT vertextCount = 0;
		UINT indexCount = 0;
	}MESHBUFFERDESC;

	struct MeshBufferInitiaizeInfo
	{
		vector<VertexTexNormalTangentBuffer> buffer;
		vector<UINT> indices;
		MESHBUFFERDESC desc = {};
	};

protected:
	explicit CMeshBuffer();
	~CMeshBuffer();

private:
	static CMeshBuffer* Create(const wstring& _filePath);

protected:
	HRESULT Initialize(const wstring& _name, wstring _filePath, void* _desc);
	void OnDestroy();

private:
	MeshBufferInitiaizeInfo CreateCube();
	MeshBufferInitiaizeInfo CreateSphere();
	MeshBufferInitiaizeInfo CreatePlane();
	MeshBufferInitiaizeInfo CreateCylinder();
	MeshBufferInitiaizeInfo CreateTriangle();
	MeshBufferInitiaizeInfo CreateObjectMesh(const string& _filePath, const _float _scaleFactor = 1.f);

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
	void* m_pVertexSysMem = nullptr;
	void* m_pIndexSysMem = nullptr;
};

NS_END

