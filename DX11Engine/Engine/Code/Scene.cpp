#include "epch.h"
#include "Scene.h"

CScene::CScene()
	: m_iSceneIndex(0)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_strSceneName(L"")
	, m_lObjectList({})
{
	m_pDevice = CGraphicDevice::GetInstance().Get_Device();
	m_pContext = CGraphicDevice::GetInstance().Get_Context();

	m_pDevice->AddRef();
	m_pContext->AddRef();
}

CScene::~CScene()
{
	SceneRelease();
}

HRESULT CScene::Initialize()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if (FAILED((*it)->Initialize()))
			return E_FAIL;
		else
			(*it)->Awake();
	}

	return S_OK;
}

void CScene::Awake()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Awake();
}

void CScene::Start()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Start();
}

void CScene::UpdateEditor()
{
}

void CScene::Update()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Update();
}

void CScene::FixedUpdate()
{
}

void CScene::LateUpdateEditor()
{
}

void CScene::LateUpdate()
{
}

void CScene::Render_Editor()
{
}

void CScene::Render_Game()
{
	auto black = ColorValue::black();
	CGraphicDevice::GetInstance().Clear_BackBuffer_View(&black);
	CGraphicDevice::GetInstance().Clear_DepthStencil_View();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		(*it)->OnPreRender();
		(*it)->Render();
		(*it)->OnPostRender();
	}

	CGraphicDevice::GetInstance().Present();
}

void CScene::SceneRelease()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		(*it)->OnDestroy();
		Safe_Release(*it);
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

void CScene::Set_Name(const wstring _name)
{
	m_strSceneName = _name;
}

const wstring& CScene::Get_Name() const
{
	return m_strName;
}

CGameObject* CScene::Add_GameObject(wstring _name)
{
	CGameObject* newObj = new CGameObject(_name, m_pDevice, m_pContext);
	newObj->AddRef();

	m_lObjectList.push_back(newObj);
	return newObj;
}

CGameObject* CScene::Instantiate(CGameObject* _gameObject)
{
	CGameObject* newObj = new CGameObject(*_gameObject);
	newObj->AddRef();

	m_lObjectList.push_back(newObj);

	return newObj;
}
