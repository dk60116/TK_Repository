#include "cpch.h"
#include "Arrow.h"

CArrow::CArrow()
{
}

CArrow::~CArrow()
{
}

CComponent* CArrow::Clone() const
{
	return nullptr;
}

HRESULT CArrow::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CArrow::Awake()
{
	__super::Awake();
}

void CArrow::Start()
{
	__super::Start();
}

void CArrow::Update()
{
	__super::Update();
}

void CArrow::OnDestroy()
{

}

void CArrow::OnCollisionEnter(CCollider* _other)
{
	__super::OnCollisionEnter(_other);
}

void CArrow::OnCollisionStay(CCollider* _other)
{
	__super::OnCollisionStay(_other);
}

void CArrow::OnCollisionExit(CCollider* _other)
{
	__super::OnCollisionExit(_other);
}
