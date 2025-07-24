#pragma once

#include "Component.h"
#include "MeshBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMeshFilter : public CComponent
{
	friend class CGameObject;

protected:
	explicit CMeshFilter();
	~CMeshFilter();

private:
	static CMeshFilter* Create();

public:
	HRESULT Initialize() override;
	void OnDestroy();

public:
	CMeshBuffer* Get_MeshBuffer() const;
	void Set_Mesh(CMeshBuffer* _buffer);

private:
	CMeshBuffer* m_pMeshBuffer;
};

NS_END

