#include "CGameObject.h"

CGameObject::CGameObject(const wstring _name, LPDIRECT3DDEVICE9 _graphicDev)
	: m_pScene(nullptr)
	, m_strName(_name)
	, m_bIsActive(true)
	, m_bIsEnable(true)
	, m_bKill(false)
	, m_isClone(false)
	, m_pGraphicDev(_graphicDev)
	, m_pTransform(nullptr)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(CGameObject& _rhs)
	: m_pScene(_rhs.m_pScene)
	, m_strName(_rhs.m_strName + L"_Clone")
	, m_bIsActive(_rhs.m_bIsActive)
	, m_bIsEnable(_rhs.m_bIsEnable)
	, m_bKill(false)
	, m_isClone(true)
	, m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_pTransform(_rhs.m_pTransform)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
	OnDestroy();
}

void CGameObject::Awake()
{
	m_pTransform = AddComponent<CTransform>();
}

void CGameObject::Start()
{
}

void CGameObject::Update()
{
	for (TRAVERSAL_ITER(m_lComponentlist, it))
	{
		if ((*it)->IsEnable())
			(*it)->Update();
	}
}

void CGameObject::FixedUpdate()
{
}

void CGameObject::LateUpdate()
{
}

void CGameObject::Render()
{
	for (TRAVERSAL_ITER(m_lComponentlist, it))
	{
		if ((*it)->IsEnable())
			(*it)->Render();
	}
}

void CGameObject::OnEnable()
{
}

void CGameObject::OnDisable()
{
}

void CGameObject::OnDestroy()
{
	Release();

	for (TRAVERSAL_ITER(m_lComponentlist, it))
	{
		Safe_Delete(*it);
	}

	m_lComponentlist.clear();
}
