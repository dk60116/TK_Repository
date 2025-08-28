#pragma once

#include "PlayerBehaviour.h"

class CPlayerBehaviour_Jump : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_Jump();
	~CPlayerBehaviour_Jump();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

private:
	_bool m_bDetached;
	vector3 m_vDirection;
};

