#pragma once

#include "Engine_Define.h"
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh
	: public CVIBuffer
{
public:
	enum MeshType { CUBE, SPHERE, CAPSULE, CYLINDER, PLANE, QUAD, CUSTOM };

public:
	CMesh();
	~CMesh();

public:
	void Awake() override;
	void Render() override;

public:
	HRESULT Ready_Buffer() override;
	void UpdateColor() override;

public:
	HRESULT Ready_Mesh();
	const MeshType& getMeshType() { return m_eMeshType; }
	void SetMeshType(const MeshType _type) { m_eMeshType = _type; }

	HRESULT Create_Mesh(const void* _pVertices, UINT vertexSize, UINT vertexCount,
		const void* _pIndices, UINT indexSize, UINT _indexCount, DWORD _fvf, D3DFORMAT _idxFormat);

private:
	HRESULT Create_Cube();
	HRESULT Create_Sphere();
	HRESULT Create_Capsule();
	HRESULT Create_Cylinder();
	HRESULT Create_Plane();
	HRESULT Create_Quad();
	HRESULT Create_Custom();

private:
	MeshType m_eMeshType;
};

END

