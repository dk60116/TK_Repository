#pragma once

#include "Component.h"
#include "MeshBuffer.h"

NS_BEGIN(Engine)

class CMeshFilter : public CComponent
{
protected:
	explicit CMeshFilter();
	~CMeshFilter();

public:
	static CMeshFilter* Create();

public:
	HRESULT Initialize() override;

public:
	void Bind_Mesh_Buffer(CMeshBuffer* _buffer);

public:
	CMeshBuffer* Get_MeshBuffer() const;

private:
	CMeshBuffer* m_pMeshBuffer;
};

NS_END

