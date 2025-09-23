#include "cpch.h"
#include "DungeonTrigger.h"
#include "DungeonGate.h"
#include "MovingPlat.h"

CDungeonTrigger::CDungeonTrigger()
	: CDungeonObject{}
	, m_bSwitchOn(false)
	, m_bSwitchComplete(false)
	, m_vLinkObjects({})
	, m_pPointLight(nullptr)
	, m_pOffTexture(nullptr)
	, m_pOnTexture(nullptr)
	, m_fLimitTime(0.f)
	, m_fPassedTime(0.f)
	, m_bDTSet(false)
{
}

CDungeonTrigger::~CDungeonTrigger()
{
}

CDungeonTrigger* CDungeonTrigger::Create()
{
	return new CDungeonTrigger();
}

CComponent* CDungeonTrigger::Clone() const
{
	CDungeonTrigger* clone = new CDungeonTrigger();

	return clone;
}

HRESULT CDungeonTrigger::Initialize(void* _desc)
{
	m_strObjName = L"DungeonTrigger";

	m_sDescription.isTrigger = true;
	m_sDescription.colliderCenter = vector3(0.f, 1.f, 0.f);
	m_sDescription.colliderSize = vector3(0.3f, 1.f, 0.3f);

	m_pOffTexture = CResources::LoadOnScene<CTexture>(L"DungeonTrigger_BaseMap (Texture)");
	m_pOnTexture = CResources::LoadOnScene<CTexture>(L"DungeonTrigger_BaseMap_On (Texture)");

	if (m_pOffTexture)
		m_pOffTexture->AddRef();

	if (m_pOnTexture)
		m_pOnTexture->AddRef();

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CDungeonTrigger::Awake()
{
	__super::Awake();

	if (!m_pBodyCollider)
	{
		m_pBodyCollider = Get_Transform()->Get_Child()->Get_GameObject()->AddComponent<CBoxCollider>();

		m_pBodyCollider->Set_Center(vector3::up() * 0.75f * (1.f / m_sDescription.scaleFactor));
		m_pBodyCollider->Set_Size(vector3(0.5f, 1.5f, 0.5f) * (1.f / m_sDescription.scaleFactor));
	}

	if (!m_pPointLight)
	{
		CGameObject* lightObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Point Light");
		lightObj->Get_Transform()->SetParent(Get_Transform());
		lightObj->Get_Transform()->Set_LocalPositionY(2.f);

		m_pPointLight = lightObj->AddComponent<CLight>();
		m_pPointLight->Set_Type(CLight::Type::Point);
		m_pPointLight->Set_Color(ColorValue(55, 55, 200, 255));

		m_pPointLight->SetEnabled(false);
	}
}

void CDungeonTrigger::Start()
{
	__super::Start();
}

void CDungeonTrigger::Update()
{
	__super::Update();

	if (m_fLimitTime > 0.f)
	{
		if (m_bOperation && !m_bSwitchComplete && m_bSwitchOn)
		{
			m_fPassedTime += DELTA_TIME;

			if (m_fPassedTime >= m_fLimitTime)
				OffSwitch();
		}
	}
}

void CDungeonTrigger::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->GetLayer() == CSceneManager::NameToLayer(L"PlayerWeapon"))
		SwitchOnEvent();
}

void CDungeonTrigger::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pOffTexture);
	Safe_Release(m_pOnTexture);

	for (TRAVERSAL_ITER(m_vLinkObjects, it))
		Safe_Release((*it));

	m_vLinkObjects.clear();
}

void CDungeonTrigger::Add_LinkObject(CDungeonObject* _obj)
{
	m_vLinkObjects.push_back(_obj);
	m_vLinkObjects.back()->AddRef();
}

void CDungeonTrigger::Set_Sibling(const _uint _index)
{
	m_iSiblingSwitchIndex = _index;
}

void CDungeonTrigger::Set_LimitTime(const _float _time)
{
	m_fLimitTime = _time;
}

void CDungeonTrigger::Set_DT()
{
	m_bDTSet = true;
}

const _bool CDungeonTrigger::Get_SwitchOn() const
{
	return m_bSwitchOn;
}

void CDungeonTrigger::SwitchOnEvent()
{
	m_bSwitchOn = true;
	m_pPointLight->SetEnabled(true);
	m_vRenderer[0]->Get_Material()->Set_Texture(m_pOnTexture);
	m_fPassedTime = 0.f;

	if (m_bDTSet)
	{
		CGameManager::GetInstance().Get_Dungeon()->OnDTSwitch();

		return;
	}

	_bool siblingOn = true;

	if (m_iSiblingSwitchIndex != -1)
	{
		auto siblingSwitchList = CGameManager::GetInstance().Get_Dungeon()->Get_DungeonTrigger(m_iSiblingSwitchIndex);

		for (TRAVERSAL_ITER(siblingSwitchList, it))
		{
			if (!(*it)->m_bSwitchOn)
				siblingOn = false;
		}
	}

	if (m_iSiblingSwitchIndex == -1 || siblingOn)
	{
		for (size_t i = 0; i < m_vLinkObjects.size(); ++i)
		{
			if (CDungeonGate* gate = dynamic_cast<CDungeonGate*>(m_vLinkObjects[i]))
				gate->Open();

			if (CMovingPlat* plat = dynamic_cast<CMovingPlat*>(m_vLinkObjects[i]))
				plat->Set_Operation(true);
		}

		vector<CDungeonTrigger*> siblingSwitchList = CGameManager::GetInstance().Get_Dungeon()->Get_DungeonTrigger(m_iSiblingSwitchIndex);

		for (TRAVERSAL_ITER(siblingSwitchList, it))
			(*it)->m_bSwitchComplete = true;
	}
}

void CDungeonTrigger::OffSwitch()
{
	m_bSwitchOn = false;
	m_pPointLight->SetEnabled(false);
	m_vRenderer[0]->Get_Material()->Set_Texture(m_pOffTexture);
}
