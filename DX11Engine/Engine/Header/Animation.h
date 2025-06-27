#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimation : public CEngineResource
{
	friend class CResources;

public:
	struct KeyFrame
	{
		double timeStamp = {};
		XMFLOAT3 position = {};
		XMFLOAT4 rotation = {};
		XMFLOAT3 scaling = {};
	};

	struct BoneAnimation
	{
		wstring boneName = {};
		vector<KeyFrame> keyFrames = {};
	};

protected:
	CAnimation();
	~CAnimation();

private:
	static CAnimation* Create(const wstring& filePath);
	HRESULT Initialize(const wstring& _filePath) override;
	void OnDestroy() override;

private:
	vector<BoneAnimation> m_vBoneAnimation;
	_float m_fDuration;
	_float m_fTicksPerSecond;
};

NS_END

