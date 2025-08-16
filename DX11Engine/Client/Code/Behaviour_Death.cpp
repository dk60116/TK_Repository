#include "cpch.h"
#include "Behaviour_Death.h"

CBehaviour_Death::CBehaviour_Death()
{
}

CBehaviour_Death::~CBehaviour_Death()
{
}

HRESULT CBehaviour_Death::Initialize(CMonster* _monster)
{
    if (FAILED(__super::Initialize(_monster)))
        return E_FAIL;

    return S_OK;
}

void CBehaviour_Death::Enter()
{
    __super::Enter();

    m_pMonster->Get_Animator()->SetLoop(false);
    m_pMonster->Get_Animator()->Play(L"Death", 0.25f);
}

void CBehaviour_Death::During()
{
    __super::During();

    if (m_fPassedTime >= 2.f)
        m_pMonster->Get_Animator()->Pause();
}

void CBehaviour_Death::Exit()
{
    __super::Exit();
}
