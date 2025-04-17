#pragma once

#include "Engine_Define.h"
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final
	: public CVIBuffer
{
public:
	enum MeshType { CUBE, SPHERE, CAPSULE, CYLINDER, PLANE, QUAD, CUSTOM };

public:
	explicit CMesh();
	~CMesh();

public:
	HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
	void UpdateColor() override;

public:
	HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 _device);
	const MeshType& getMeshType() { return m_eMeshType; }
	void SetMeshType(const MeshType _type) { m_eMeshType = _type; }

	HRESULT Create_Mesh(const void* _pVertices, UINT vertexSize, UINT vertexCount,
		const void* _pIndices, UINT indexSize, UINT _indexCount, DWORD _fvf, D3DFORMAT _idxFormat);

private:
	HRESULT Create_Cube(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Sphere(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Capsule(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Cylinder(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Plane(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Quad(LPDIRECT3DDEVICE9 _device);
	HRESULT Create_Custom(LPDIRECT3DDEVICE9 _device);

private:
	MeshType m_eMeshType;
};

END

