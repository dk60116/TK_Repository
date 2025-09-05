#include "cpch.h"
#include "MovingPlat.h"

CMovingPlat::CMovingPlat()
	: m_pBodyCollider(nullptr)
	, m_vRout({})
	, m_fMoveSpeed(2.f)
	, m_iTargetStep(1)
	, m_bBackStep(false)
	, m_fWaitTime(0.f)
{
}

CMovingPlat::~CMovingPlat()
{
}

CMovingPlat* CMovingPlat::Create()
{
	return new CMovingPlat;
}

CComponent* CMovingPlat::Clone() const
{
	CMovingPlat* clone = new CMovingPlat();

	return clone;
}

HRESULT CMovingPlat::Initialize()
{
	m_strObjName = L"MovingPlat";

	m_sDescription.scaleFactor = 0.04f;
	m_sDescription.isTrigger = true;
	m_sDescription.colliderCenter = vector3::up() * 0.5f;
	m_sDescription.colliderSize = vector3(2.f, 1.f, 2.f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CMovingPlat::Awake()
{
	__super::Awake();

	if (!m_pBodyCollider)
	{
		m_pBodyCollider = Get_Transform()->Get_Child()->Get_GameObject()->AddComponent<CBoxCollider>();

		m_pBodyCollider->Set_Center(vector3::up() * 0.25f * (1.f / m_sDescription.scaleFactor));
		m_pBodyCollider->Set_Size(vector3(3.f, 0.5f, 3.f) * (1.f / m_sDescription.scaleFactor));
	}
}

void CMovingPlat::Start()
{
	__super::Start();
}

void CMovingPlat::Update()
{
	__super::Update();
	
	CTransform* myTF = Get_Transform();
	const vector3 myPos = Get_Transform()->Get_Position();

	if (m_iTargetStep < static_cast<_byte>(m_vRout.size()))
	{
		const vector3& targetPos = m_vRout[m_iTargetStep];

		if (vector3::Distance(myPos, targetPos) > 0.01f)
		{
			const vector3 moveValue = (targetPos - myPos).normalized() * m_fMoveSpeed * DELTA_TIME;
			myTF->Add_Position(moveValue);
			if (m_bPlayerEnter)
				CGameManager::GetInstance().Get_Player()->Get_Transform()->Add_Position(moveValue);
		}
		else
		{
			if (!m_bBackStep)
			{
				if (m_iTargetStep == static_cast<_byte>(m_vRout.size() - 1))
					m_bBackStep = true;
			}
			else
			{
				if (m_iTargetStep == 0)
					m_bBackStep = false;
			}

			m_fWaitTime += DELTA_TIME;

			if ((m_iTargetStep != 0 && m_iTargetStep != m_vRout.size() - 1) || m_fWaitTime >= 3.f)
			{
				m_fWaitTime = 0.f;

				if (!m_bBackStep)
					++m_iTargetStep;
				else
					--m_iTargetStep;
			}
		}
	}
}

void CMovingPlat::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bPlayerEnter = true;
}

void CMovingPlat::OnTriggerExit(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bPlayerEnter = false;
}

void CMovingPlat::OnDestroy()
{
	__super::OnDestroy();
}

void CMovingPlat::SetMoveSpeed(const _float _value)
{
	m_fMoveSpeed = _value;
}

void CMovingPlat::AddRout(const vector3 _pos)
{
	m_vRout.push_back(_pos);
}
