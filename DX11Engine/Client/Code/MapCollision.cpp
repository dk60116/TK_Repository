#include "cpch.h"
#include "MapCollision.h"
#include "Map.h"

CMapCollision::CMapCollision()
	: m_pMap(nullptr)
	, m_bSelectViewMode(nullptr)
	, m_vColliderList()
	, m_iIDCount(0)
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
		if (CInput::GetKeyDown_Editor(N))
		{
			SpawnTempCollider(MapCollisionType::Wall);
		}

		if (CInput::GetKeyDown_Editor(B))
		{
			SpawnTempCollider(MapCollisionType::Floor);
		}

		if (CInput::GetKeyDown_Editor(M))
		{
			CBoxCollider* proto = CEditor::Get_SelectedGameObject()->GetComponent<CBoxCollider>();

			if (proto)
			{
				CollidersInfo info = {};

				switch (proto->Get_GameObject()->GetTag())
				{
				case 1:
					info.type = 1;
					break;
				case 2:
					info.type = 0;
					break;
				}

				info.id = m_iIDCount;
				XMStoreFloat4x4(&info.matrix, proto->Get_Transform()->Get_WorldMatrix());

				auto copy = CopyTempCollider(info);

				CEditor::Set_SelectedGameObject(copy->Get_GameObject());
			}
		}
	}

	if (CInput::GetKeyDown_Editor(O))
		m_bSelectViewMode = !m_bSelectViewMode;

	UpdateViewMode(m_bSelectViewMode);
}

void CMapCollision::OnDestroy()
{
	Safe_Release(m_pMap);
}

HRESULT CMapCollision::SaveColliders(const wstring _filePath)
{
	if (m_vColliderList.size() <= 0)
		return E_FAIL;

	for (size_t i = 0; i < m_vColliderList.size(); ++i)
	{
		_float4x4 w = {};
		XMStoreFloat4x4(&w, m_vColliderList[i]->Get_Transform()->Get_WorldMatrix());
		m_vInfoList[i].matrix = w;
	}

	vector<CollidersInfo> infoList = {};

	for (size_t i = 0; i < m_vColliderList.size(); ++i)
	{
		if (m_vColliderList[i]->Get_GameObject()->ActiveSelf())
			infoList.push_back(m_vInfoList[i]);
	}

	ofstream out(_filePath, ios::binary | ios::trunc);

	if (!out.is_open())
	{
		CDebug::LogError(L"SaveCollisionObjectTransformInfos failed - can not open: " + _filePath);
		return E_FAIL;
	}

	_uint count = static_cast<_uint>(infoList.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(_uint));

	for (_uint i = 0; i < count; ++i)
	{
		CollidersInfo info = infoList[i];
		out.write(reinterpret_cast<const char*>(&info.type), sizeof(BYTE));
		out.write(reinterpret_cast<const char*>(&info.id), sizeof(_int));
		out.write(reinterpret_cast<const char*>(&info.matrix), sizeof(_float4x4));
	}

	out.close();

	CDebug::Log(L"Save complete mapCollisondata: " + _filePath);

	return S_OK;
}

vector<CMapCollision::CollidersInfo> CMapCollision::ReadColliderInfo(const wstring _binFileName)
{
	ifstream in(L"BinaryAssets/SceneData/" + _binFileName + L".mapcoldata", ios::binary);

	if (!in.is_open())
		return {};

	vector<CollidersInfo> infoList = {};

	_uint count = 0;
	in.read(reinterpret_cast<char*>(&count), sizeof(_uint));
	infoList.resize(count);

	for (_uint i = 0; i < count; ++i)
	{
		in.read(reinterpret_cast<char*>(&infoList[i].type), sizeof(BYTE));
		in.read(reinterpret_cast<char*>(&infoList[i].id), sizeof(_int));
		in.read(reinterpret_cast<char*>(&infoList[i].matrix), sizeof(_float4x4));
	}

	return infoList;
}

void CMapCollision::LoadColliders(const vector<CollidersInfo>& _info)
{
	for (_uint i = 0; i < _info.size(); ++i)
	{
		SpawnDataCollider(_info[i]);
	}
}

void CMapCollision::Set_Map(CMap* _map)
{
	m_pMap = _map;

	m_pMap->AddRef();
}

CBoxCollider* CMapCollision::SpawnTempCollider(const MapCollisionType _type)
{
	CScene* pScene = m_pGameObject->Get_Scene();

	CGameObject* newColObj = pScene->Add_GameObject(m_pMap->Get_MapName() + (_type == MapCollisionType::Wall ? L"_Wall" : L" _Floor") + L' ' + to_wstring(m_iIDCount));
	CBoxCollider* newCol = newColObj->AddComponent<CBoxCollider>();
	newColObj->SetLayer(L"Map");
	newColObj->SetTag(_type == MapCollisionType::Wall ? L"Wall" : L"Floor");

	CollidersInfo info = {};

	switch (_type)
	{
	case MapCollisionType::Floor:
		info.type = 1;
		break;

	case MapCollisionType::Wall:
		info.type = 2;
		break;
	}

	info.id = m_iIDCount;

	m_vColliderList.push_back(newCol);
	m_vInfoList.push_back(info);

	++m_iIDCount;

	return newCol;
}

CBoxCollider* CMapCollision::SpawnDataCollider(const CollidersInfo& _info)
{
	CScene* pScene = m_pGameObject->Get_Scene();

	MapCollisionType type = static_cast<MapCollisionType>(_info.type);

	CGameObject* newColObj = pScene->Add_GameObject(m_pMap->Get_MapName() + (type == MapCollisionType::Wall ? L"_Wall" : L" _Floor") + L' ' + to_wstring(_info.id));
	CBoxCollider* newCol = newColObj->AddComponent<CBoxCollider>();
	newColObj->SetLayer(L"Map");
	newColObj->SetTag(type == MapCollisionType::Wall ? L"Wall" : L"Floor");

	m_vColliderList.push_back(newCol);
	m_vInfoList.push_back(_info);

	_matrix world = XMLoadFloat4x4(&_info.matrix);
	newColObj->Get_Transform()->SetTransformForMatrix(world);

	++m_iIDCount;

	return newCol;
}

CBoxCollider* CMapCollision::CopyTempCollider(CollidersInfo& _proto)
{
	CScene* pScene = m_pGameObject->Get_Scene();

	MapCollisionType type = static_cast<MapCollisionType>(_proto.type);

	CGameObject* newColObj = pScene->Add_GameObject(m_pMap->Get_MapName() + (type == MapCollisionType::Wall ? L"_Wall" : L" _Floor") + L' ' + to_wstring(m_iIDCount));
	CBoxCollider* newCol = newColObj->AddComponent<CBoxCollider>();
	newColObj->SetLayer(L"Map");
	newColObj->SetTag(type == MapCollisionType::Wall ? L"Wall" : L"Floor");
	newColObj->Get_Transform()->SetTransformForMatrix(XMLoadFloat4x4(&_proto.matrix));

	m_vColliderList.push_back(newCol);
	m_vInfoList.push_back(_proto);

	++m_iIDCount;

	return newCol;
}

void CMapCollision::UpdateViewMode(const _bool _selected)
{
	CBoxCollider* selectedCol = nullptr;

	if (CEditor::Get_SelectedGameObject())
		selectedCol = CEditor::Get_SelectedGameObject()->GetComponent<CBoxCollider>();

	if (_selected)
	{
		for (TRAVERSAL_ITER(m_vColliderList, it))
		{
			if (selectedCol)
				(*it)->SetEnabled(selectedCol == (*it));
		}
	}
	else
	{
		for (TRAVERSAL_ITER(m_vColliderList, it))
			(*it)->SetEnabled(true);
	}
}
