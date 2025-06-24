#pragma once

#include "Component.h"
#include "MeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshFilter : public CComponent
{
protected:
	explicit CMeshFilter();
	~CMeshFilter();

public:
	static CMeshFilter* Create();

public:
	HRESULT Initialize() override;
	void OnDestroy();

public:
	void CreateMeshBuffer(const wstring _shape);

public:
	CMeshBuffer* Get_MeshBuffer() const;
	void Set_MeshBuffer(CMeshBuffer* _buffer);

private:
	CMeshBuffer* m_pMeshBuffer;
};

NS_END

