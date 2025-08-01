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

HRESULT CWolf::Initialize()
{
	m_strSkinnedMeshBufferName = L"Wolf_Model (MeshBuffer)";

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CDebug::Log(CResources::GetInstance().LoadSkinnedBonesOnScene(m_strSkinnedMeshBufferName).size());

	m_pAnimator->Add_Animation(L"Idle", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Wolf_Idle (Animation)"));
	m_pAnimator->SetLoop(true);

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

	if (CInput::GetInstance().GetKeyDown(K))
		m_pAnimator->Play(L"Idle");
}

void CWolf::OnDestroy()
{
	__super::OnDestroy();
}
