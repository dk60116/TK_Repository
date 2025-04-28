#include "CMaterial.h"
#include "CSpriteRenderer.h"

CMaterial::CMaterial()
	: m_sMaterial({})
	, m_sOptions({})
{
	ZeroMemory(&m_sMaterial, sizeof(D3DMATERIAL9));
}

CMaterial::~CMaterial()
{
}

void CMaterial::Apply(LPDIRECT3DDEVICE9 _pDevice)
{
	if (!_pDevice)
		return;

	m_sMaterial.Diffuse = m_sOptions.diffuseColor.dvColor();
	m_sMaterial.Ambient = m_sOptions.ambientColor.dvColor();
	m_sMaterial.Specular = m_sOptions.specularColor.dvColor();
	m_sMaterial.Emissive = D3DXCOLOR(0, 0, 0, 1);
	m_sMaterial.Power = m_sOptions.power;

	_pDevice->SetMaterial(&m_sMaterial);
}

void CMaterial::Destroy()
{
	Release();
}
