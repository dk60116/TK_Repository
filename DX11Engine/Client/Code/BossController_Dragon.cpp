#include "cpch.h"
#include "BossController_Dragon.h"
#include "DragonBHV_Idle.h"
#include "DragonBHV_Wakeup.h"
#include "DragonBHV_Landing.h"
#include "DragonBHV_GetHit.h"
#include "DragonBHV_RotationFly.h"
#include "DragonBHV_ShootFireBall.h"
#include "DragonBHV_Grounding.h"
#include "DragonBHV_Tracking.h"
#include "DragonBHV_Attack.h"
#include "DragonBHV_CombatWait.h"
#include "DragonBHV_Dead.h"

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

HRESULT CBossController_Dragon::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CBossController_Dragon::Awake()
{
	__super::Awake();

	AddBehaviour<CDragonBHV_Idle>((_uint)DIdle);
	AddBehaviour<CDragonBHV_Wakeup>((_uint)Wakeup);
	AddBehaviour<CDragonBHV_Landing>((_uint)Landing);
	AddBehaviour<CDragonBHV_GetHit>((_uint)DGetHit);
	AddBehaviour<CDragonBHV_RotationFly>((_uint)RotateFly);
	AddBehaviour<CDragonBHV_ShootFireBall>((_uint)ShootFireBall);
	AddBehaviour<CDragonBHV_Grounding>((_uint)Grounding);
	AddBehaviour<CDragonBHV_Tracking>((_uint)DTracking);
	AddBehaviour<CDragonBHV_Attack>((_uint)Attack_Ground);
	AddBehaviour<CDragonBHV_CombatWait>((_uint)DCombatWait);
	AddBehaviour<CDragonBHV_Dead>((_uint)DDeath);

	ChangeState((_uint)DIdle, nullptr, true);
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
