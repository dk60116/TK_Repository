#include "CAnimator.h"
#include "CResourceManager.h"
#include "CGameObject.h"

CAnimator::CAnimator()
	: m_pCurrentAnimation(nullptr)
	, m_pRenderer(nullptr)
	, m_bIsPlaying(true)
	, m_fCurrnetTime(0.f)
{
}

CAnimator::~CAnimator()
{
	OnDestroy();
}

void CAnimator::Awake()
{
	Awake_();
}

void CAnimator::Start()
{
	Start_();
}

void CAnimator::Reset()
{
	Reset_();
}

void CAnimator::Update()
{
	Update_();

	if (m_bIsPlaying && m_pCurrentAnimation)
	{
		m_fCurrnetTime += DELTA_TIME;

		while (m_fCurrnetTime >= m_pCurrentAnimation->m_fFrameInterval)
		{
			m_fCurrnetTime -= m_pCurrentAnimation->m_fFrameInterval;
			++(m_pCurrentAnimation->m_iCurrentFrame);

			if (m_pCurrentAnimation->m_iCurrentFrame >= m_pCurrentAnimation->m_iFrameCount)
			{
				if (m_pCurrentAnimation->m_bLoop)
					m_pCurrentAnimation->m_iCurrentFrame = 0;
				else
					m_pCurrentAnimation->m_iCurrentFrame = m_pCurrentAnimation->m_iFrameCount - 1;
			}
		}
	}
}

void CAnimator::FixedUpdate()
{
	FixedUpdate_();
}

void CAnimator::LateUpdate()
{
	LateUpdate_();
}

void CAnimator::OnEnable()
{
	OnEnable_();
}

void CAnimator::OnDisable()
{
	OnDisable_();
}

void CAnimator::Render(HDC _hdc)
{
}

void CAnimator::OnCollisionEnter(CCollider* _other)
{
}

void CAnimator::OnCollisionStay(CCollider* _other)
{
}

void CAnimator::OnCollisionExit(CCollider* _other)
{
}

void CAnimator::OnDestroy()
{
	OnDestroy_();

	for (TRAVERSAL_ITER(m_mAnimationList, it))
		Safe_Delete((it)->second);

	m_mAnimationList.clear();
}

void CAnimator::OnMouseEnter()
{
}

void CAnimator::OnMouseOver()
{
}

void CAnimator::OnMouseExit()
{
}

void CAnimator::GetAnimationDisplay(HDC& _hDC, CTransform& _tf, vector2 _position, float _size, const ColorValue _tpColor)
{
	if (m_pCurrentAnimation != nullptr)
		m_pCurrentAnimation->DisplayFrame(_hDC, _tf, _position, _size, _tpColor);
}

void CAnimator::Play()
{
	if (m_pCurrentAnimation)
		m_bIsPlaying = true;
}

void CAnimator::Pause()
{
	if (m_pCurrentAnimation)
		m_bIsPlaying = false;
}

void CAnimator::Stop()
{
	if (m_pCurrentAnimation)
	{
		m_bIsPlaying = false;
		m_pCurrentAnimation->m_iCurrentFrame = 0;
	}
}

void CAnimator::CreateAnimation(wstring _name, wstring _sprite, int _count, vector2 _offset, vector2Int _xyLength, int _yValue, float _frameInterval, bool _loop)
{
	CAnimation* _newAnimation = new CAnimation();
	
	m_mAnimationList.insert({ _name, _newAnimation });

	HDC memDC = CResourceManager::GetInstance().Find_Sprite(_sprite);

	_newAnimation->Init(this, memDC, _count, _offset, _xyLength, _yValue, _frameInterval, _loop);

	if (m_mAnimationList.size() == 1)
		SetAnimation(_name);
}

void CAnimator::SetAnimation(wstring _lastName)
{
	if (m_pCurrentAnimation != m_mAnimationList[_lastName])
	{
		m_pCurrentAnimation = m_mAnimationList[_lastName];
		m_pCurrentAnimation->m_iCurrentFrame = 0;
	}
}

void CAnimator::SetAnimation(int _ySlice)
{
	for (TRAVERSAL_ITER(m_mAnimationList, it))
	{
		if ((*it).second->m_iYvalue == _ySlice)
			SetAnimation((*it).first);
	}
}

const int CAnimator::getCurrentAnimationFrame()
{
	return m_pCurrentAnimation->m_iCurrentFrame;
}
