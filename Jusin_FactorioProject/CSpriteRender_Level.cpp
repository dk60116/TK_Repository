#include "CSpriteRender_Level.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CRandom.h"
#include "CInput.h"
#include "CScreen.h"

CSpriteRender_Level::CSpriteRender_Level()
	: m_pRenderer(nullptr)
	, m_v2StartPos()
	, m_v2EndPos()
	, m_bRandomX(false)
	, m_iXMax(16)
	, m_iX(0)
{
}

CSpriteRender_Level::CSpriteRender_Level(CSpriteRenderer* _renderer)
	: m_pRenderer(_renderer)
	, m_v2StartPos()
	, m_v2EndPos()
	, m_bRandomX(false)
	, m_iXMax(16)
	, m_iX(0)
{
}

CSpriteRender_Level::~CSpriteRender_Level()
{
	OnDestroy();
}

void CSpriteRender_Level::Awake()
{
	Awake_();

	if (m_bRandomX)
		m_iX = CRandom::GetInstance().Range(0, m_iXMax);

	m_pRenderer->m_v2StartPoint.x = m_iX;
}

void CSpriteRender_Level::Start()
{
	Start_();
}

void CSpriteRender_Level::Reset()
{
	Reset_();
}

void CSpriteRender_Level::Update()
{
	Update_();

	if (CSceneManager::GetInstance().getCurrentscene()->getMainCamera() == nullptr)
		return;

	CCamera* mainCam = CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	vector2 screen = vector2(CScreen::GetInstance().getOptions().width, CScreen::GetInstance().getOptions().height) / (mainCam->getSize());

	if (getTransform().getPosition().x < (int)mainCam->getTransform().getPosition().x - screen.x / UNIT)
		return;
	if (getTransform().getPosition().x > (int)mainCam->getTransform().getPosition().x + screen.x / UNIT)
		return;
	if (getTransform().getPosition().y < (int)mainCam->getTransform().getPosition().y - (screen.y / UNIT) - 1)
		return;
	if (getTransform().getPosition().y > (int)mainCam->getTransform().getPosition().y + (screen.y / UNIT) + 1)
		return;

	float scale = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	if (scale < 0.6f)
	{
		m_pRenderer->m_v2StartPoint.x = m_v2EndPos[2].x * m_iX;
		m_pRenderer->m_v2StartPoint.y = m_v2StartPos[2].y;
		m_pRenderer->m_v2EndPoint.x = m_v2EndPos[2].x;
		m_pRenderer->m_v2EndPoint.y = m_v2EndPos[2].y;
	}
	else if (scale < 1.25f)
	{
		m_pRenderer->m_v2StartPoint.x = m_v2EndPos[1].x * m_iX;
		m_pRenderer->m_v2StartPoint.y = m_v2StartPos[1].y;
		m_pRenderer->m_v2EndPoint.x = m_v2EndPos[1].x;
		m_pRenderer->m_v2EndPoint.y = m_v2EndPos[1].y;
	}
	else
	{
		m_pRenderer->m_v2StartPoint.x = m_v2EndPos[0].x * m_iX;
		m_pRenderer->m_v2StartPoint.y = m_v2StartPos[0].y;
		m_pRenderer->m_v2EndPoint.x = m_v2EndPos[0].x;
		m_pRenderer->m_v2EndPoint.y = m_v2EndPos[0].y;
	}
}

void CSpriteRender_Level::FixedUpdate()
{
	FixedUpdate_();
}

void CSpriteRender_Level::LateUpdate()
{
	LateUpdate_();
}

void CSpriteRender_Level::OnEnable()
{
	OnEnable_();
}

void CSpriteRender_Level::OnDisable()
{
	OnDisable_();
}

void CSpriteRender_Level::Render(HDC _hDC)
{
}

void CSpriteRender_Level::OnCollisionEnter(CCollider* _other)
{
}

void CSpriteRender_Level::OnCollisionStay(CCollider* _other)
{
}

void CSpriteRender_Level::OnCollisionExit(CCollider* _other)
{
}

void CSpriteRender_Level::OnDestroy()
{
	OnDestroy_();
}

void CSpriteRender_Level::OnMouseEnter()
{
}

void CSpriteRender_Level::OnMouseOver()
{
}

void CSpriteRender_Level::OnMouseExit()
{
}

void CSpriteRender_Level::SetRenderPos(const vector2Int _start[], const vector2Int _end[])
{
	for (size_t i = 0; i < 3; ++i)
	{
		m_v2StartPos[i] = _start[i];
		m_v2EndPos[i] = _end[i];
	}
}
