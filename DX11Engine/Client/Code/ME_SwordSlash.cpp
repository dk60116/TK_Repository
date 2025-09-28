#include "cpch.h"
#include "ME_SwordSlash.h"

CME_SwordSlash::CME_SwordSlash()
{
}

CME_SwordSlash::~CME_SwordSlash()
{
}

CME_SwordSlash* CME_SwordSlash::Create()
{
	return new CME_SwordSlash();
}

CComponent* CME_SwordSlash::Clone() const
{
	CME_SwordSlash* clone = new CME_SwordSlash();

	return clone;
}

HRESULT CME_SwordSlash::Initialize(void* _desc)
{
	m_strEffectName = L"SwordSlash";

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_vMesheList[0]->Get_Material()->Set_BaseColor(ColorValue(255, 255, 255, 100).f4Color());

	m_pGameObject->SetActive(false);

	return S_OK;
}

void CME_SwordSlash::Awake()
{
	__super::Awake();

	Get_Transform()->SetParent(CGameManager::GetInstance().Get_Player()->Get_Transform());
	Get_Transform()->Set_LocalPosition(0.f, 1.f, 0.f);
}

void CME_SwordSlash::Start()
{
	__super::Awake();
}

void CME_SwordSlash::Update()
{
	__super::Update();

	_float a = 0.f;

	if (m_fTime <= 0.3f)
		a = m_fTime * 5.f;
	else
		a = 1.f - m_fTime * 1.5f;

	a = std::clamp(a, 0.f, 0.4f);

	_float4 col = _float4(1.f, 1.f, 1.f, a);
	m_vMesheList[0]->Get_Material()->Set_BaseColor(col);

	if (m_fTime > 1.f)
		m_pGameObject->SetActive(false);
}

void CME_SwordSlash::OnEnable()
{
	__super::OnEnable();
	m_vMesheList[0]->Get_Transform()->Set_LocalScale(0.7f);
}

void CME_SwordSlash::OnDestroy()
{
	__super::OnDestroy();
}

void CME_SwordSlash::PlayCombo(const _uint _index)
{
	m_pGameObject->SetActive(true);
	m_fTime = 0.f;

	switch (_index)
	{
	case 0:
		Get_Transform()->Set_LocalEulerAngles(17.3f, 42.8f, -37.5f);
		break;
	case 1:
		Get_Transform()->Set_LocalEulerAngles(15.5f, 140.f, 17.f);
		break;
	case 2:
		Get_Transform()->Set_LocalEulerAngles(-14.f, 140.f, -17.f);
		break;
	case 3:
		Get_Transform()->Set_LocalEulerAngles(0.f, -50.f, 0.f);
		break;
	default:
		break;
	}
}
