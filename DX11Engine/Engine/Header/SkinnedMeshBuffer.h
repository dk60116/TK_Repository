#pragma once

#include "MeshBuffer.h"

class ENGINE_DLL CSkinnedMeshBuffer : public CMeshBuffer
{
private:
	explicit CSkinnedMeshBuffer();
	~CSkinnedMeshBuffer();

public:
	static CSkinnedMeshBuffer* Create();

	HRESULT Initialize(const string& _filePath, float _scaleFactor = 1.0f);
	void Render();
	void OnDestroy();
};

