#include "cpch.h"
#include "Wolf.h"

CWolf::CWolf()
	: CMonster{}
{
}

CWolf::~CWolf()
{
}

CWolf* CWolf::Create()
{
	return new CWolf();
}

CComponent* CWolf::Clone() const
{
	CWolf* clone = new CWolf();

	return clone;
}

HRESULT CWolf::Initialize()
{
	m_strSkinnedMeshBufferName = L"Wolf_Model (MeshBuffer)";

	m_sOptions.colliderCenter = vector3(0.f, 0.6f, 0.2f);
	m_sOptions.colliderSize = vector3(0.5f, 1.f, 1.5f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CWolf::Awake()
{
	__super::Awake();
}

void CWolf::Start()
{
	__super::Start();
}

void CWolf::Update()
{
	__super::Update();
}

void CWolf::OnDestroy()
{
	__super::OnDestroy();
}

void CWolf::OnCollisionEnter(CCollider* _other)
{
	CDebug::LogError("Enter");
}

void CWolf::OnCollisionStay(CCollider* _other)
{
}
