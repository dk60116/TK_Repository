#include "cpch.h"
#include "FootSwitch.h"
#include "DungeonGate.h"
#include "MovingPlat.h"

CFootSwitch::CFootSwitch()
	: m_pBodyTransform(nullptr)
	, m_bSwitchOn(nullptr)
	, m_pStepCollider(nullptr)
	, m_pEnteredObj(nullptr)
	, m_bObjectEnter(false)
	, m_bPressed(false)
	, m_bPrevPressed(false)
	, m_bSwitchComplete(false)
	, m_bMustDetect(nullptr)
{
}

CFootSwitch::~CFootSwitch()
{
}

CFootSwitch* CFootSwitch::Create()
{
	return new CFootSwitch();
}

CComponent* CFootSwitch::Clone() const
{
	CFootSwitch* clone = new CFootSwitch();

	return clone;
}

HRESULT CFootSwitch::Initialize(void* _desc)
{
	m_strObjName = L"Dungeon_FootSwitchPlat";

	m_sDescription.isTrigger = true;
	m_sDescription.colliderCenter = vector3::up() * 0.4f;
	m_sDescription.colliderSize = vector3(0.7f, 1.f, 0.7f);

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CFootSwitch::Awake()
{
	__super::Awake();

	auto bodyRen = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(L"Dungeon_FootSwitch_Model (Mesh Buffer)"), 0.012f);
	bodyRen[0]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"Dungeon_FootSwitch_BaseMap (Texture)"));

	m_pBodyTransform = bodyRen[0]->Get_Transform();

	if (m_pCollider)
		m_pCollider->SetTrigger(true);

	if (!m_pStepCollider)
	{
		m_pStepCollider = m_pBodyTransform->Get_GameObject()->AddComponent<CSphereCollider>();
		m_pStepCollider->Set_Size(2.3f * 100.f);
		m_pStepCollider->Set_Center(vector3::down() * 0.7f * 100.f);
	}

	m_pStepCollider->Get_GameObject()->SetLayer(L"Map");

	m_pAudioSource->SetLoop(true);
}

void CFootSwitch::Update()
{
	__super::Update();

	if (!m_bPressed && !m_bObjectEnter)
		m_pBodyTransform->Set_LocalPositionY(Lerp(m_pBodyTransform->Get_LocalPosition().y, 0.f, 4 * DELTA_TIME));

	if (!m_bPressed && m_bObjectEnter)
	{
		if (m_pBodyTransform->Get_LocalPosition().y >= -0.57f)
			m_pBodyTransform->Add_PositionY(-0.5f * DELTA_TIME);
	}

	if (m_pBodyTransform->Get_LocalPosition().y <= -0.55f)
	{
		m_bPressed = true;
	}

	if (m_bPressed && !m_bPrevPressed)
		SwitchOnEvent();

	m_bPrevPressed = m_bPressed;
}

void CFootSwitch::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player") || _other->Get_GameObject()->CompareTag(L"DungeonCube"))
	{
		m_pEnteredObj = _other->Get_GameObject();
		m_bObjectEnter = true;
		PlaySoundEffect(L"FootSwitch");
	}
}

void CFootSwitch::OnTriggerExit(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player") || _other->Get_GameObject()->CompareTag(L"DungeonCube"))
	{
		m_bObjectEnter = false;

		if (m_bMustDetect)
		{
			if (m_pEnteredObj == _other->Get_GameObject())
			{
				if (!m_bSwitchComplete)
				{
					m_bPressed = false;
					m_bSwitchOn = false;
				}

				m_pEnteredObj = nullptr;

				StopSound();
			}
		}
		else
			StopSound();
	}
}

void CFootSwitch::OnDestroy()
{
	__super::OnDestroy();

	for (TRAVERSAL_ITER(m_vLinkObjects, it))
		Safe_Release(*it);

	m_vLinkObjects.clear();
}

void CFootSwitch::Set_Gate(vector<CDungeonObject*>& _object)
{
	m_vLinkObjects = _object;

	for (TRAVERSAL_ITER(m_vLinkObjects, it))
	{
		if (*it)
			(*it)->AddRef();
	}
}

void CFootSwitch::SetMustDetect(const _bool _value)
{
	m_bMustDetect = _value;
}

void CFootSwitch::SwitchOnEvent()
{
	m_bSwitchOn = true;

	_bool siblingOn = true;

	if (m_iSiblingSwitchIndex != -1)
	{
		auto siblingSwitchList = CGameManager::GetInstance().Get_Dungeon()->Get_FootSwitch(m_iSiblingSwitchIndex);

		for (TRAVERSAL_ITER(siblingSwitchList, it))
		{
			if (!(*it)->m_bSwitchOn)
				siblingOn = false;
		}
	}

	if (m_iSiblingSwitchIndex == -1 || siblingOn)
	{
		CGameManager::GetInstance().Get_Dungeon()->PlayPuzzleClearSound();

		for (size_t i = 0; i < m_vLinkObjects.size(); ++i)
		{
			if (CDungeonGate* gate = dynamic_cast<CDungeonGate*>(m_vLinkObjects[i]))
				gate->Open();

			if (CMovingPlat* plat = dynamic_cast<CMovingPlat*>(m_vLinkObjects[i]))
				plat->Set_Operation(true);
		}

		vector<CFootSwitch*> siblingSwitchList = CGameManager::GetInstance().Get_Dungeon()->Get_FootSwitch(m_iSiblingSwitchIndex);

		for (TRAVERSAL_ITER(siblingSwitchList, it))
		{
			(*it)->m_bSwitchComplete = true;
			StopSound();
		}
	}

	StopSound();
}
