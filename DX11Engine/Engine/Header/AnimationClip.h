#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimationClip : public CEngineResource
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

	struct BoneTransform
	{
		_float3 pos = { 0.f , 0.f, 0.f };
		_float4 rot = { 0.f, 0.f, 0.f, 1.f };
		_float3 scale = { 1.f ,1.f, 1.f };
	};

	struct BoneAnimation
	{
		wstring boneName = L"";
		vector<KeyFrame> keyFrames = {};
	};

protected:
	CAnimationClip();
	~CAnimationClip();

public:
	void Sample(_float _timeSec, unordered_map<wstring, BoneTransform>& _out) const;
	const _bool IsLoop() const;
	_float Get_Duration() const;

protected:
	static CAnimationClip* Create();
	HRESULT Initialize(const wstring& _name, const wstring& _filePath, void* _desc) override;
	void OnDestroy() override;


private:
	vector<BoneAnimation> m_vBoneAnimation;
	_bool m_bLoopTime;
	_float m_fDuration;
	_float m_fTicksPerSecond;
};

NS_END

