#pragma once

#include "Object.h"

class ENGINE_DLL CMaterial final : public UObject
{
private:
	explicit CMaterial();
	virtual ~CMaterial();

public:
	static CMaterial* Create(const wstring _path = L"");

private:
	HRESULT Initialize(const wstring _path = L"");
	void OnDestroy();

public:
	void Bind(const _fmatrix _world, const _cmatrix _view, const _cmatrix _projection);

public:
	class CTexture* Get_Texture(_int _index) const;

public:
	HRESULT Load_Shader(const wstring& _path);
	void Set_DiffuseColor(const ColorValue& _color);
	void Set_Texture(CTexture* _texture, _int _index);

private:
	HRESULT Create_ConstantBuffer();

	void Bind_Shader();
	void Bind_Texture();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pCameraBuffer;
	ID3D11Buffer* m_pMaterialBuffer;

	vector<class CTexture*> m_vTextureList;
	ColorValue m_vDiffuseColor;
};

