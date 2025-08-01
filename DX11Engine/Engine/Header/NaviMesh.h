#pragma once

#include "MeshBuffer.h"

NS_BEGIN(Engine)

NS_BEGIN(EngineAI)

class ENGINE_DLL CNaviMesh final : public CMeshBuffer
{
	friend class CResources;

private:
	CNaviMesh();
	~CNaviMesh();

private:
	static CNaviMesh* Create();
};

NS_END

NS_END

