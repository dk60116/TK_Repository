#include "cpch.h"
#include "FootSwitch.h"
#include "DungeonGate.h"

CFootSwitch::CFootSwitch()
	: m_pBodyTransform(nullptr)
	, m_pStepCollider(nullptr)
	, m_bObjectEnter(false)
	, m_bPressed(false)
	, m_bPrevPressed(false)
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

	auto bodyRen = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(L"Dungeon_FootSwitch_Model (MeshBuffer)"), 0.012f);
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

	if (m_pBodyTransform->Get_LocalPosition().y < -0.55f)
		m_bPressed = true;

	if (m_bPressed && !m_bPrevPressed)
		OpenGate();

	m_bPrevPressed = m_bPressed;
}

void CFootSwitch::OnTriggerStay(CCollider* _other)
{
}

void CFootSwitch::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bObjectEnter = true;
}

void CFootSwitch::OnTriggerExit(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bObjectEnter = false;
}

void CFootSwitch::OnDestroy()
{
	__super::OnDestroy();

	for (TRAVERSAL_ITER(m_vGates, it))
		Safe_Release(*it);

	m_vGates.clear();
}

void CFootSwitch::Set_Gate(vector<class CDungeonGate*>& _gates)
{
	m_vGates = _gates;

	for (TRAVERSAL_ITER(m_vGates, it))
	{
		if (*it)
			(*it)->AddRef();
	}
}

void CFootSwitch::OpenGate()
{
	for (TRAVERSAL_ITER(m_vGates, it))
	{
		(*it)->Open();
	}
}
