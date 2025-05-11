#include "CAudioManager.h"

CAudioManager::CAudioManager()
    : m_pSystem(nullptr)
{

}

CAudioManager::~CAudioManager()
{
    Release();
}

void CAudioManager::Init()
{
    if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
    {
#ifdef  _DEBUG
        CConsole::Print("FMOD System create failed");
#endif //  _DEBUG
        return;
    }

    // 최대 16개 채널, 기본 설정
    if (m_pSystem->init(16, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
    {
#ifdef  _DEBUG
        CConsole::Print("FMOD System initialize failed");
#endif //  _DEBUG
        return;
    }

#ifdef  _DEBUG
    CConsole::Print("FMOD System initialize completed");
#endif //  _DEBUG
}

void CAudioManager::Update()
{
}

void CAudioManager::Release()
{
    if (m_pSystem)
    {
        m_pSystem->close();
        m_pSystem->release();
        m_pSystem = nullptr;
    }
}
