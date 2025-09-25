#pragma once
#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAudioClip final : public CEngineResource
{
	friend class CResources;

public:
	CAudioClip();
	~CAudioClip();

protected:
	static CAudioClip* Create();
	HRESULT Initialize(const wstring& _name, const wstring& _filePath, void* _desc) override;
	void OnDestroy() override;

public:
	FMOD::Sound* getSound() const;

private:
	FMOD::Sound* m_pSound;
};

NS_END

