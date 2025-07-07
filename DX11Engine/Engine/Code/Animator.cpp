#include "epch.h"
#include "Animator.h"
#include "SkinnedMeshRenderer.h"

CAnimator::CAnimator()
	: m_pSkinnedRenderer(nullptr)
	, m_mAnimationList({})
	, m_pCrtAnimation(nullptr)
    , m_bIsPlaying(false)
	, m_bLoop(false)
	, m_fCurrentTime(0.f)
	, m_fPlaybackSpeed(1.f)
	, m_vFinalBoneMatrix({})
{
	m_strName = L"Animator";
}

CAnimator::~CAnimator()
{
}

CAnimator* CAnimator::Create()
{
	return new CAnimator();
}

HRESULT CAnimator::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CAnimator::Awake()
{
	if (!m_pSkinnedRenderer)
	{
		m_pSkinnedRenderer = m_pGameObject->GetComponent<CSkinnedMeshRenderer>();

		if (m_pSkinnedRenderer)
			m_pSkinnedRenderer->AddRef();
	}
}

void CAnimator::Update()
{
    if (!m_bIsPlaying || !m_pSkinnedRenderer || !m_pCrtAnimation)
        return;

	m_fCurrentTime += DELTA_TIME * m_fPlaybackSpeed;

	const float duration = m_pCrtAnimation->Get_Duration();
	if (m_bLoop)
		m_fCurrentTime = fmodf(m_fCurrentTime, duration);
	else if (m_fCurrentTime >= duration)
	{
		m_fCurrentTime = duration;
		m_bIsPlaying = false;
	}

	// 2) 현재 시각의 키프레임 샘플링 --------------------
	unordered_map<wstring, CAnimation::BoneTransform> sampled;
	m_pCrtAnimation->Sample(m_fCurrentTime, sampled);

	// 3) 각 본 CTransform 갱신 -------------------------
	const uint32_t boneCount = m_pSkinnedRenderer->Get_BoneCount();

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		CTransform* bone = m_pSkinnedRenderer->Get_BoneTransform(i);
		if (!bone) continue;

		const wstring& name = m_pSkinnedRenderer->Get_BoneName(i);

		auto it = sampled.find(name);
		if (it == sampled.end()) continue;

		const auto& bt = it->second;

		bone->Set_LocalPosition(bt.pos);
		bone->Set_LocalQuaternion(bt.rot);
		bone->Set_LocalScale(bt.scale);
	}
}

void CAnimator::OnDestroy()
{
	for (TRAVERSAL_ITER(m_mAnimationList, it))
		Safe_Release((*it).second);

	Safe_Release(m_pSkinnedRenderer);
}

void CAnimator::Add_Animation(const wstring& _animName, CAnimation* _anim)
{
	if (_anim == nullptr)
		return;

	m_mAnimationList[_animName] = _anim;

	_anim->AddRef();
}

void CAnimator::Set_PlaybackSpeed(const _float _value)
{
	m_fPlaybackSpeed = _value;
}

void CAnimator::Play(const wstring& _animName)
{
	auto iter = m_mAnimationList.find(_animName);
	
	if (iter != m_mAnimationList.end())
	{
		m_pCrtAnimation = iter->second;
		m_fCurrentTime = 0.f;
		m_bIsPlaying = true;
	}
}

void CAnimator::Pause()
{
	m_bIsPlaying = false;
}

void CAnimator::Stop()
{
	m_fCurrentTime = 0.f;

	Update();

	m_bIsPlaying = false;
}

void CAnimator::SetLoop(const _bool _loop)
{
	m_bLoop = _loop;
}

void CAnimator::SetSpeed(const _float _value)
{
	m_fPlaybackSpeed = _value;
}

CAnimation* CAnimator::Get_CurrentAnimation()
{
	return m_pCrtAnimation;
}
