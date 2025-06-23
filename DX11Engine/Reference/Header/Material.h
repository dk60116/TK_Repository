#pragma once

#include "Object.h"

class ENGINE_DLL CMaterial final : public UObject
{
public:
	explicit CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial();

public:
	HRESULT Initialize(const std::wstring& vsPath, const std::wstring& psPath);
	void OnDestroy();

public:
	void Render();

public:
	void Set_DiffuseColor(const ColorValue& color);
	void Set_DiffuseTexture(ID3D11ShaderResourceView* pSRV);

private:
	HRESULT Load_Shader(const std::wstring& vsPath, const std::wstring& psPath);
	HRESULT Create_ConstantBuffer();

	void Bind_Shader();
	void Bind_Texture();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pConstantBuffer;

	ID3D11ShaderResourceView* m_pDiffuseSRV;
	ColorValue m_vDiffuseColor;
};

