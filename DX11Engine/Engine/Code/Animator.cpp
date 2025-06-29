#include "epch.h"
#include "Animator.h"
#include "SkinnedMeshRenderer.h"

CAnimator::CAnimator()
	: m_pSkinnedMesh(nullptr)
	, m_mAnimationList({})
	, m_pCrtAnimation(nullptr)
    , m_bIsPlaying(false)
	, m_bLoop(false)
	, m_fCurrentTime(0.f)
	, m_fPlaybackSpeed(1.f)
	, m_mBoneTransformLIst({})
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

	if (!m_pSkinnedMesh)
	{
		m_pSkinnedMesh = m_pGameObject->GetComponent<CSkinnedMeshRenderer>();
		
		if (m_pSkinnedMesh)
			m_pSkinnedMesh->AddRef();
	}

	return S_OK;
}

void CAnimator::Update()
{
	if (m_pCrtAnimation == nullptr || !m_bIsPlaying)
		return;

	m_fCurrentTime += m_fPlaybackSpeed * DELTA_TIME;

	const _float duration = m_pCrtAnimation->Get_Duration();

	if (m_fCurrentTime > duration)
	{
		if (m_bLoop)
			m_fCurrentTime = fmod(m_fCurrentTime, duration);
		else
		{
			m_fCurrentTime = duration;
			m_bIsPlaying = false;
		}
	}

	m_mBoneTransformLIst.clear();
	m_pCrtAnimation->Sample(m_fCurrentTime, m_mBoneTransformLIst);

	if (m_pSkinnedMesh)
	{
		for (auto& [boneName, transform] : m_mBoneTransformLIst)
		{
			auto* bone = m_pSkinnedMesh->FindBone(boneName);
			
			if (bone)
			{
				bone->Set_LocalPosition(transform.pos);
				bone->Set_LocalQuaternion(transform.rot);
				bone->Set_LocalScale(transform.scale);
			}
		}
	}
}

void CAnimator::OnDestroy()
{
	for (TRAVERSAL_ITER(m_mAnimationList, it))
		Safe_Release((*it).second);

	Safe_Release(m_pSkinnedMesh);
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

void CAnimator::SetLoop(const _bool _loop)
{
	m_bLoop = _loop;
}

CAnimation* CAnimator::Get_CurrentAnimation()
{
	return m_pCrtAnimation;
}
