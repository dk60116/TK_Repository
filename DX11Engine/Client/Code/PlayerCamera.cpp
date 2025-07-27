#include "cpch.h"
#include "PlayerCamera.h"

CPlayerCamera::CPlayerCamera()
	: m_pPlayer(nullptr)
{
}

CPlayerCamera::~CPlayerCamera()
{
}

HRESULT CPlayerCamera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CPlayerCamera::Awake()
{
}

void CPlayerCamera::Start()
{
}

void CPlayerCamera::Update()
{
}

void CPlayerCamera::OnDestroy()
{
}