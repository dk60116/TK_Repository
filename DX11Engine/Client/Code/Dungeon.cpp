#include "cpch.h"
#include "Dungeon.h"

CDungeon::CDungeon()
{
}

CDungeon::~CDungeon()
{
}

CDungeon* CDungeon::Create()
{
    return new CDungeon();
}

CComponent* CDungeon::Clone() const
{
    CDungeon* clone = new CDungeon();

    return clone;
}

HRESULT CDungeon::Initialize()
{
    m_strMapName = L"Dungeon";
    m_sOptions.scaleFactor = 0.015f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    return S_OK;
}

void CDungeon::Awake()
{
    __super::Awake();
}

void CDungeon::Start()
{
    __super::Start();
}

void CDungeon::Update()
{
    __super::Update();
}

void CDungeon::OnDestroy()
{
    __super::OnDestroy();
}
