#pragma once
#include "PlayerBehaviour.h"

class CPlayerBehaviour_SpinAttack final : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_SpinAttack();
	~CPlayerBehaviour_SpinAttack();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

};

