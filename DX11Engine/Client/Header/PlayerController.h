#pragma once
#include "Component.h"

class CPlayerController final : public CComponent
{
public:
	enum PlayerState { None, Idle, Move, Sword, Bow, Jump, Ladder, GetHit, Death };
	enum class TurnDir { None, Left, Right };

	typedef struct moveDirectionDesc
	{
		vector3 moveDirection = {};
		vector3 prevMoveDir = {};
		_float rotateDirection = 0.f;
		_float prevRotateDir = 0.f;
	}MOVEDESC;

private:
	explicit CPlayerController();
	~CPlayerController();

public:
	static CPlayerController* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	class CPlayer* Get_Player();
	void Set_Player(CPlayer* _player);

private:
	template<typename T>
	void AddBehaviour(const PlayerState _state);

public:
	const PlayerState Get_State() const;
	void Set_Focus(CTransform* _target);
	void ChangeState(const PlayerState _state, const _bool _forceEnter = false, void* _enterDesc = nullptr);
	void ForceChangeState(const PlayerState _state, void* _enterDesc = nullptr);
	const _bool IsDamaged() const;
	void SetDamaged(const _bool _value);
	const _bool IsDead() const;
	void SetDead();

public:
	const vector3& Get_MoveDirection();
	void Set_MoveDirection(const vector3& _value);
	const float Get_RotateDirection();
	void Set_RotationDiretion(const _float _value);
	MOVEDESC& Get_MoveDesc();

private:
	void UpdateControleState();

private:
	PlayerState m_eCrtState;
	class CPlayer* m_pPlayer;
	CTransform* m_pFocusTransform;

	map<_uint, class CPlayerBehaviour*> m_mBehaviourList;
	CPlayerBehaviour* m_pCrtBehaviour;

	_bool m_bDamaged, m_bDead;

	MOVEDESC m_sMoveDesc;
};

template<typename T>
inline void CPlayerController::AddBehaviour(const PlayerState _state)
{
	T* b = new T();
	b->Initialize(m_pPlayer);
	b->AddRef();
	m_mBehaviourList.emplace(static_cast<_uint>(_state), b);
}
