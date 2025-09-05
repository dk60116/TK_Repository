#include "cpch.h"
#include "BossController_Dragon.h"
#include "DragonBHV_Idle.h"
#include "DragonBHV_Wakeup.h"
#include "DragonBHV_Landing.h"

CBossController_Dragon::CBossController_Dragon()
{
}

CBossController_Dragon::~CBossController_Dragon()
{
}

CBossController_Dragon* CBossController_Dragon::Create()
{
	return new CBossController_Dragon();
}

CComponent* CBossController_Dragon::Clone() const
{
	CBossController_Dragon* clone = new CBossController_Dragon();

	return clone;
}

HRESULT CBossController_Dragon::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CBossController_Dragon::Awake()
{
	__super::Awake();

	AddBehaviour<CDragonBHV_Idle>((_uint)Idle);
	AddBehaviour<CDragonBHV_Wakeup>((_uint)Wakeup);
	AddBehaviour<CDragonBHV_Landing>((_uint)Landing);

	Change_State((_uint)Idle, nullptr, true);
}

void CBossController_Dragon::Start()
{
	__super::Start();
}

void CBossController_Dragon::Update()
{
	__super::Update();
}

void CBossController_Dragon::OnDestroy()
{
	__super::OnDestroy();
}
