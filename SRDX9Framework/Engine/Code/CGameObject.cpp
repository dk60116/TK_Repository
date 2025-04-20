#include "CGameObject.h"

CGameObject::CGameObject(wstring _name, LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pScene(nullptr)
	, m_strName(_name)
	, m_bIsActive(true)
	, m_bIsEnable(true)
	, m_bKill(false)
	, m_isClone(false)
	, m_pGraphicDev(_pGraphicDev)
	, m_pTransform(nullptr)
{
	m_strName = L"GameObject";
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pScene(_rhs.m_pScene)
	, m_strName(_rhs.m_strName + L"_Clone")
	, m_bIsActive(_rhs.m_bIsActive)
	, m_bIsEnable(_rhs.m_bIsEnable)
	, m_bKill(false)
	, m_isClone(true)
	, m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_pTransform(_rhs.m_pTransform)
{
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

void CGameObject::RenderEditor()
{
	for (TRAVERSAL_ITER(m_lComponentlist, it))
	{
		if ((*it)->IsEnable())
			(*it)->RenderEditor();
	}
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
		(*it)->OnDestroy();
		Safe_Release(*it);
	}

	m_lComponentlist.clear();
}
