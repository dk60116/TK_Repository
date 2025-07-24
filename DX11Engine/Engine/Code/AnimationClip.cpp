#include "epch.h"
#include "AnimationClip.h"

CAnimationClip::CAnimationClip()
	: m_vBoneAnimation({})
	, m_bLoopTime(false)
	, m_fDuration(0.f)
	, m_fTicksPerSecond(0.f)
{
	m_strName = L"Animation";
}

CAnimationClip::~CAnimationClip()
{
	OnDestroy();
}

void CAnimationClip::Sample(_float _timeSec, unordered_map<wstring, BoneTransform>& _out) const
{
	if (m_vBoneAnimation.empty() || m_fDuration == 0.f)
		return;

	double ticks = _timeSec * m_fTicksPerSecond;
	double time = fmod(ticks, m_fDuration);

	_out.clear();
	_out.reserve(m_vBoneAnimation.size());

	for (const auto& ba : m_vBoneAnimation)
	{
		const auto& keys = ba.keyframes;
		if (keys.empty()) continue;

		size_t i1 = 0, i2 = 0;
		while (i2 < keys.size() && time >= keys[i2].timeStamp) { i1 = i2++; }

		if (i2 >= keys.size()) { i2 = i1; }          // 끝 구간
		float span = float(keys[i2].timeStamp - keys[i1].timeStamp);
		float  t = span > 0.f ? float((time - keys[i1].timeStamp) / span) : 0.f;

		// 보간
		BoneTransform bt;
		XMStoreFloat3(&bt.pos,
			XMVectorLerp(XMLoadFloat3(&keys[i1].position), XMLoadFloat3(&keys[i2].position), t));

		XMStoreFloat4(&bt.rot,
			XMQuaternionNormalize(
				XMQuaternionSlerp(XMLoadFloat4(&keys[i1].rotation), XMLoadFloat4(&keys[i2].rotation), t)));

		XMStoreFloat3(&bt.scale,
			XMVectorLerp(XMLoadFloat3(&keys[i1].scaling), XMLoadFloat3(&keys[i2].scaling), t));

		_out.emplace(ba.boneName, bt);
	}
}

const _bool CAnimationClip::IsLoop() const
{
	return m_bLoopTime;
}

_float CAnimationClip::Get_Duration() const
{
	return m_fDuration;
}

CAnimationClip* CAnimationClip::Create()
{
	return new CAnimationClip();
}

void CAnimationClip::OnDestroy()
{
	__super::Release();

	m_vBoneAnimation.clear();

	m_fDuration = 0.f;
	m_fTicksPerSecond = 0.f;
}

HRESULT CAnimationClip::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
	if (FAILED(__super::Initialize(_name, _filePath, _desc)))
		return E_FAIL;

	ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

	if (!device)
		return E_FAIL;

	string filePathUTF8 = CEngineString::WStringToString(_filePath);

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile
	(
		filePathUTF8,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | 
		aiProcess_GenSmoothNormals | 
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded | 
		aiProcess_LimitBoneWeights
	);

	if (!scene || !scene->HasAnimations())
	{
		CDebug::LogError(L"Assimp Animation load failed: " + m_strFilePath);
		return E_FAIL;
	}

	const aiAnimation* anim = scene->mAnimations[0];

	m_fDuration = static_cast<float>(anim->mDuration);
	m_fTicksPerSecond = static_cast<float>(anim->mTicksPerSecond != 0.0f ? anim->mTicksPerSecond : 25.0f);

	m_vBoneAnimation.clear();

	for (_uint c = 0; c < anim->mNumChannels; ++c)
	{
		const aiNodeAnim* channel = anim->mChannels[c];
		BoneAnimation boneAnim;
		string bn = channel->mNodeName.C_Str();
		boneAnim.boneName = wstring(bn.begin(), bn.end());

		size_t numKeys = max<size_t>
		(
			size_t(channel->mNumPositionKeys),
			max<size_t>
			(
				size_t(channel->mNumRotationKeys),
				size_t(channel->mNumScalingKeys)
			)
		);

		const _float scaleFactor = _desc ? *static_cast<_float*>(_desc) : 1.f;

		for (size_t k = 0; k < numKeys; ++k)
		{
			Keyframe keyframe = {};

			if (k < channel->mNumPositionKeys)
			{
				const aiVector3D& pos = channel->mPositionKeys[k].mValue;
				keyframe.position = _float3(pos.x * scaleFactor, pos.y * scaleFactor, pos.z * scaleFactor);
				keyframe.timeStamp = channel->mPositionKeys[k].mTime;
			}

			if (k < channel->mNumRotationKeys)
			{
				const aiQuaternion& rot = channel->mRotationKeys[k].mValue;
				keyframe.rotation = _float4(rot.x, rot.y, rot.z, rot.w);
				keyframe.timeStamp = channel->mRotationKeys[k].mTime;
			}

			if (k < channel->mNumScalingKeys)
			{
				const aiVector3D& scl = channel->mScalingKeys[k].mValue;
				keyframe.scaling = _float3(scl.x, scl.y, scl.z);
				keyframe.timeStamp = channel->mScalingKeys[k].mTime;
			}

			boneAnim.keyframes.push_back(keyframe);
		}

		m_vBoneAnimation.push_back(move(boneAnim));
	}

	return S_OK;
}
