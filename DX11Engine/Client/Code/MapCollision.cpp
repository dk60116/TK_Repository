#include "cpch.h"
#include "MapCollision.h"
#include "Map.h"

CMapCollision::CMapCollision()
	: m_pMap(nullptr)
	, m_mColliderList()
{
}

CMapCollision::~CMapCollision()
{
}

CMapCollision* CMapCollision::Create()
{
	return new CMapCollision();
}

CComponent* CMapCollision::Clone() const
{
	CMapCollision* clone = new CMapCollision();

	return clone;
}

HRESULT CMapCollision::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CMapCollision::Awake()
{
}

void CMapCollision::Update()
{
	if (CInput::GetKey_Editor(CONTROL))
	{
		CScene* pScene = m_pGameObject->Get_Scene();

		if (CInput::GetKeyDown_Editor(V))
		{
			CGameObject* newColObj = pScene->Add_GameObject(m_pMap->Get_MapName() + L" Wall");
			CBoxCollider* newCol = newColObj->AddComponent<CBoxCollider>();
			newColObj->SetLayer(L"Map");
			newColObj->SetTag(L"Wall");
		}

		if (CInput::GetKeyDown_Editor(B))
		{
			CGameObject* newColObj = pScene->Add_GameObject(m_pMap->Get_MapName() + L" Floor");
			CBoxCollider* newCol = newColObj->AddComponent<CBoxCollider>();
			newColObj->SetLayer(L"Map");
			newColObj->SetTag(L"Floor");
		}
	}
}

void CMapCollision::OnDestroy()
{
	Safe_Release(m_pMap);
}

HRESULT CMapCollision::SaveColliders(const wstring _filePath)
{
	ofstream out(_filePath, ios::out);

	if (!out.is_open())
	{
		CDebug::LogError(L"SaveCollisionObjectTransformInfos failed - can not open: " + _filePath);
		return E_FAIL;
	}

	_uint count = static_cast<_uint>(m_mColliderList.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(_uint));

	for (_uint i = 0; i < count; ++i)
	{
		CollidersInfo info = _infoList[i];
	}

	out.close();

	CDebug::Log(L"Save complete mapCollisondata: " + _filePath);

	return S_OK;
}

void CMapCollision::Set_Map(CMap* _map)
{
	m_pMap = _map;

	m_pMap->AddRef();
}
