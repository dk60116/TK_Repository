#include "epch.h"
#include "Animation.h"

CAnimation::CAnimation()
	: m_vBoneAnimation({})
	, m_fDuration(0.f)
	, m_fTicksPerSecond(0.f)
{
}

CAnimation::~CAnimation()
{
	OnDestroy();
}

CAnimation* CAnimation::Create(const wstring& filePath)
{
	return new CAnimation();
}

void CAnimation::OnDestroy()
{
	__super::Release();

	m_vBoneAnimation.clear();

	m_fDuration = 0.f;
	m_fTicksPerSecond = 0.f;
}

HRESULT CAnimation::Initialize(const wstring& _filePath)
{
	if (FAILED(__super::Initialize(_filePath)))
		return E_FAIL;

	ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

	if (!device)
		return E_FAIL;

	string filePathUTF8(m_strFilePath.begin(), m_strFilePath.end());

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile
	(
		filePathUTF8,
		aiProcess_Triangulate | aiProcess_LimitBoneWeights
	);

	if (!scene || !scene->HasAnimations())
	{
		CDebug::LogError("Assimp Animation load failed");
		return E_FAIL;
	}

	const aiAnimation* anim = scene->mAnimations[0];

	m_fTicksPerSecond = static_cast<float>(anim->mDuration);
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

		for (size_t k = 0; k < numKeys; ++k)
		{
			KeyFrame keyframe = {};

			if (k < channel->mNumPositionKeys)
			{
				const aiVector3D& pos = channel->mPositionKeys[k].mValue;
				keyframe.position = _float3(pos.x, pos.y, pos.z);
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

			boneAnim.keyFrames.push_back(keyframe);
		}

		m_vBoneAnimation.push_back(move(boneAnim));
	}

	return S_OK;
}
