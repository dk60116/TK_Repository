#pragma once
#include "PlayerBehaviour.h"

class CPlayerBehaviour_Bow final : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_Bow();
	~CPlayerBehaviour_Bow();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

	_bool m_bAiming;
};

