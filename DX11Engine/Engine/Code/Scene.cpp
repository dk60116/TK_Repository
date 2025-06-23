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
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->FixedUpdate();
}

void CScene::LateUpdateEditor()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->LateUpdate_Editor();
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
		(*it)->OnPreCull();
		(*it)->OnPreRender();
		(*it)->Render();
		(*it)->OnPostRender();
	}

	CGraphicDevice::GetInstance().Present();
}

void CScene::SceneRelease()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		Safe_Release(*it);

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
	m_lObjectList.back()->Initialize();

	return newObj;
}

CCamera* CScene::Get_Camera() const
{
	return m_lCameraList.back();
}

CCamera* CScene::Get_Camera(const _int _index) const
{
	_int i = 0;

	if (m_lCameraList.size() <= 0)
		return nullptr;

	for (TRAVERSAL_ITER(m_lCameraList, it))
	{
		++i;

		if (_index == i)
			return (*it);
	}

	return m_lCameraList.back();
}

list<class CCamera*>& CScene::Get_CameraList()
{
	return m_lCameraList;
}

CCamera* CScene::Add_Camera(CCamera* _camera)
{
	m_lCameraList.push_back(_camera);

	return _camera;
}

CGameObject* CScene::Instantiate(CGameObject* _gameObject)
{
	CGameObject* newObj = new CGameObject(*_gameObject);
	newObj->AddRef();

	m_lObjectList.push_back(newObj);

	return newObj;
}
