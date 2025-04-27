#include "CScene.h"
#include "CTime.h"
#include "CInput.h"
#include "CManagement.h"
#include "CGameObject.h"
#include "CEditorCamera.h"

CScene::CScene()
	: m_pGraphicDev(nullptr)
	, m_lObjectList()
	, m_vCameraList({})
	, m_vLightList({})
	, m_sOptions({})
{
}

CScene::~CScene()
{
	Destroy();
}

void CScene::Awake()
{
}

void CScene::Start()
{
}

void CScene::UpdateEditor()
{
	UpdateAllLight();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->isActive() && (*it)->isEnable())
			(*it)->UpdateEditor();
	}
}

void CScene::Update()
{
	UpdateAllLight();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->isActive() && (*it)->isEnable())
			(*it)->Update();
	}
}

void CScene::FixedUpdate()
{
}

void CScene::LateUpdateEditor()
{
	
}

void CScene::LateUpdate()
{
	vector<CGameObject*> killObjList = {};

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->isActive() && (*it)->isEnable())
			(*it)->LateUpdate();

		if ((*it)->m_bKill)
			killObjList.push_back(*it);
	}

	for (TRAVERSAL_ITER(killObjList, it))
	{
		SafeDestroyObject(*it);
	}
}

void CScene::Render_Editor()
{
	Render_Grid();

	CCamera* editorCam = &CManagement::GetInstance().getEditorCamera(); // 에디터 카메라 기준
	vector3 cam_pos = editorCam->getTransform().getPosition();
	vector3 cam_forward = editorCam->getTransform().getDirections().forward;

	vector<CGameObject*> sortedRenderList;

	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (auto& obj : m_lObjectList)
		{
			if (obj->isActive() && obj->isEnable())
				sortedRenderList.push_back(obj);
		}
	}

	// 깊이 기준 정렬
	sort(sortedRenderList.begin(), sortedRenderList.end(),
		[&](CGameObject* a, CGameObject* b)
		{
			float depthA = vector3::dot((a->getTransform().getPosition() - cam_pos), cam_forward);
			float depthB = vector3::dot((b->getTransform().getPosition() - cam_pos), cam_forward);
			return depthA > depthB;
		});

	for (auto& obj : sortedRenderList)
	{
		obj->RenderEditor();
	}
}

void CScene::Render_Game()
{
	if (m_vCameraList.empty())
		return;

	CCamera* mainCam = m_vCameraList.back(); // 마지막 카메라 기준
	vector3 cam_pos = mainCam->getTransform().getPosition();
	vector3 cam_forward = mainCam->getTransform().getDirections().forward;

	vector<CGameObject*> sortedRenderList;

	for (auto& obj : m_lObjectList)
	{
		if (obj->isActive() && obj->isEnable())
			sortedRenderList.push_back(obj);
	}

	// 깊이 기준 정렬
	sort(sortedRenderList.begin(), sortedRenderList.end(),
		[&](CGameObject* a, CGameObject* b)
		{
			float depthA = vector3::dot((a->getTransform().getPosition() - cam_pos), cam_forward);
			float depthB = vector3::dot((b->getTransform().getPosition() - cam_pos), cam_forward);
			return depthA > depthB;
		});

	for (auto& obj : sortedRenderList)
	{
		obj->Render();
	}
}

void CScene::SceneRelease()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->GetComponent<CEditorCamera>())
			continue;

		(*it)->OnDestroy();
		Safe_Release(*it);
	}

	Safe_Release(m_pGraphicDev);

	m_lObjectList.clear();
	m_vCameraList.clear();
	m_vLightList.clear();
}

void CScene::Destroy()
{
	Release();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		(*it)->OnDestroy();
		Safe_Release(*it);
	}

	m_lObjectList.clear();

	m_lObjectList.clear();
	m_vCameraList.clear();
	m_vLightList.clear();

	Safe_Release(m_pGraphicDev);

	Release();
}

CGameObject* CScene::AddObject(wstring _objName, Layer _layer)
{
	CGameObject* obj = new CGameObject(_objName, CManagement::GetInstance().getGraphicDevice());
	obj->AddRef();
	obj->SetScene(this);
	m_lObjectList.push_back(obj);
	obj->Awake();

	return obj;
}

void CScene::AddCamera(CCamera* _cam)
{
	if (_cam)
		m_vCameraList.push_back(_cam);
}

void CScene::AddLight(CLight* _light)
{
	if (_light)
	{
		_light->SetIndex((DWORD)m_vLightList.size());
		m_vLightList.push_back(_light);
	}
}

void CScene::UpdateSceneCameraResolution(const vector2Int _resolution)
{
	CManagement::GetInstance().getEditorCamera().ResetAspectFromResolution(_resolution);
}

void CScene::UpdateAllCameraResolution(const vector2Int _resolution)
{
	for (TRAVERSAL_ITER(m_vCameraList, it))
		(*it)->ResetAspectFromResolution(_resolution);
}

vector<CGameObject*> CScene::getRootObjects()
{
	vector<CGameObject*> result = {};

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->getTransform().isRoot())
			result.push_back(*it);
	}

	return result;
}

CGameObject* CScene::FindGameObject(const wstring _name)
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->getName() == _name)
			return *it;
	}

	return nullptr;
}

void CScene::Render_Grid()
{
	const int GRID_HALF = 50;
	const float GRID_SPACING = 1.f;
	const D3DCOLOR gridColor = D3DCOLOR_XRGB(100, 100, 100);
	const int lineCount = (GRID_HALF * 2 + 1) * 2;

	VTXLINE* pVertices = new VTXLINE[lineCount * 2];
	int idx = 0;

	// Z 방향 그리드 (X 고정)
	for (int i = -GRID_HALF; i <= GRID_HALF; ++i)
	{
		float x = i * GRID_SPACING;
		pVertices[idx++] = { D3DXVECTOR3(x, 0.f, -GRID_HALF * GRID_SPACING), gridColor };
		pVertices[idx++] = { D3DXVECTOR3(x, 0.f, GRID_HALF * GRID_SPACING), gridColor };
	}

	// X 방향 그리드 (Z 고정)
	for (int i = -GRID_HALF; i <= GRID_HALF; ++i)
	{
		float z = i * GRID_SPACING;
		pVertices[idx++] = { D3DXVECTOR3(-GRID_HALF * GRID_SPACING, 0.f, z), gridColor };
		pVertices[idx++] = { D3DXVECTOR3(GRID_HALF * GRID_SPACING, 0.f, z), gridColor };
	}

	LPDIRECT3DDEVICE9 pDevice = CManagement::GetInstance().getGraphicDevice();

	// ===== 뷰-프로젝션 설정 =====
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pDevice->SetTransform(D3DTS_VIEW, &CManagement::GetInstance().getEditorCamera().getViewMatrix());
	pDevice->SetTransform(D3DTS_PROJECTION, &CManagement::GetInstance().getEditorCamera().getProjMatrix());

	// ===== 그리기 =====
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetFVF(FVF_LINE);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineCount, pVertices, sizeof(VTXLINE));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	delete[] pVertices;
}

void CScene::SafeDestroyObject(CGameObject* _obj)
{
	if (!_obj) return;

	CTransform* tf = &_obj->getTransform();
	if (!tf) return;

	auto childrenCopy = tf->getChilds();

	for (auto& childTf : childrenCopy)
	{
		if (childTf)
		{
			CGameObject* childObj = childTf->getObject();
			if (childObj)
			{
				SafeDestroyObject(childObj);
			}
		}
	}

	tf->SetParent(nullptr);

	tf->getChilds().clear();

	_obj->OnDestroy();
	Safe_Release(_obj);

	m_lObjectList.remove(_obj);
}

void CScene::UpdateAllLight()
{
	for (TRAVERSAL_ITER(m_vLightList, it))
		(*it)->Apply();
}
