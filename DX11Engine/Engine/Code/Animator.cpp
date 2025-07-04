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

	if (!m_pSkinnedRenderer)
	{
		m_pSkinnedRenderer = m_pGameObject->GetComponent<CSkinnedMeshRenderer>();
		
		if (m_pSkinnedRenderer)
			m_pSkinnedRenderer->AddRef();
	}

	return S_OK;
}

void CAnimator::Update()
{
    if (!m_bIsPlaying || !m_pSkinnedRenderer || !m_pCrtAnimation)
        return;

    m_fCurrentTime += DELTA_TIME * m_fPlaybackSpeed;
    const float duration = m_pCrtAnimation->Get_Duration();

    if (m_bLoop)
        m_fCurrentTime = fmod(m_fCurrentTime, duration);
    else if (m_fCurrentTime >= duration)
    {
        m_fCurrentTime = duration;
        m_bIsPlaying = false;
    }

    unordered_map<wstring, CAnimation::BoneTransform> local;
    m_pCrtAnimation->Sample(m_fCurrentTime, local);

    const uint32_t boneCount = m_pSkinnedRenderer->Get_BoneCount();
    m_vFinalBoneMatrix.assign(boneCount, XMMatrixIdentity());

    for (uint32_t i = 0; i < boneCount; ++i)
    {
        const wstring& name = m_pSkinnedRenderer->Get_BoneName(i);
        const CTransform* node = m_pSkinnedRenderer->Get_BoneTransform(i);

        XMMATRIX M = node->Get_LocalMatrix();
        if (auto it = local.find(name); it != local.end())
        {
            const auto& bt = it->second;
            M = XMMatrixScaling(bt.scale.x, bt.scale.y, bt.scale.z) *
                XMMatrixRotationQuaternion(XMLoadFloat4(&bt.rot)) *
                XMMatrixTranslation(bt.pos.x, bt.pos.y, bt.pos.z);
        }

        for (auto* p = node->Get_Parent(); p; p = p->Get_Parent())
            M *= p->Get_LocalMatrix();

        M *= XMLoadFloat4x4(&m_pSkinnedRenderer->Get_BoneOffsetMatrix(i));
        m_vFinalBoneMatrix[i] = XMMatrixTranspose(M); // VS column-major
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

void CAnimator::SetLoop(const _bool _loop)
{
	m_bLoop = _loop;
}

CAnimation* CAnimator::Get_CurrentAnimation()
{
	return m_pCrtAnimation;
}
