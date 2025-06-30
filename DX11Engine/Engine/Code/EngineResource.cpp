#include "epch.h"
#include "EngineResource.h"

CEngineResource::CEngineResource()
	: m_strResourceName({})
	, m_strFilePath({})
{
}

CEngineResource::~CEngineResource()
{
	OnDestroy();
}

HRESULT CEngineResource::Initialize(const wstring& _filePath, void* _desc)
{
	m_strFilePath = _filePath;

	return S_OK;
}

const wstring& CEngineResource::Get_ResourceName() const
{
	return m_strName;
}

HRESULT CEngineResource::Load(const wstring& path)
{
	return S_OK;
}

void CEngineResource::OnDestroy()
{
}