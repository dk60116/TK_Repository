#include "cpch.h"
#include "PlayerBehaviour_Ladder.h"

PlayerBehaviour_Ladder::PlayerBehaviour_Ladder()
{
}

PlayerBehaviour_Ladder::~PlayerBehaviour_Ladder()
{
}

HRESULT PlayerBehaviour_Ladder::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return  S_OK;

	return S_OK;
}

void PlayerBehaviour_Ladder::Enter(void* _desc)
{
	__super::Enter();
}

void PlayerBehaviour_Ladder::During()
{
	__super::During();
}

void PlayerBehaviour_Ladder::Exit()
{
	__super::Exit();
}
