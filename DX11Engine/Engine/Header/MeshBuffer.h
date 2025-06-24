#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshBuffer final : public UObject
{
	friend class CMeshFilter;

public:
	typedef struct MeshBufferDescription
	{
		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		UINT vertexSize = 0;
		UINT vertextCount = 0;
		UINT* indices = nullptr;
		UINT indexCount = 0;
	}MESHBUFFERDESC;

private:
	explicit CMeshBuffer();
	~CMeshBuffer();

private:
	HRESULT Initialize(const void* _vertices, MESHBUFFERDESC _description);
	void OnDestroy();

public:
	static CMeshBuffer* CreateCube(CMeshFilter* _filter);
	static CMeshBuffer* CreateSphere(CMeshFilter* _filter);
	static CMeshBuffer* CreatePlane(CMeshFilter* _filter);
	static CMeshBuffer* CreateCylinder(CMeshFilter* _filter);
	static CMeshBuffer* CreateObjectMesh(CMeshFilter* _filter);

public:
	void Render();

public:
	ID3D11Buffer* Get_VertexBuffer();
	ID3D11Buffer* Get_IndexBuffer();
	const MESHBUFFERDESC& Get_Info();

private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	class CMeshFilter* m_pFilter;

	MESHBUFFERDESC m_sInfo;

private:
	void* m_pVertexSysMem = nullptr;
	void* m_pIndexSysMem = nullptr;
};

NS_END

