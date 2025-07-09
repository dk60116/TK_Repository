#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMaterial final : public CEngineResource
{
	friend class CResources;

public:
	typedef struct Material
	{
		CShader* shaderPointer;
	} MATERIALDESC;

private:
	explicit CMaterial();
	virtual ~CMaterial();

private:
	static CMaterial* Create(const wstring _path = L"");

private:
	HRESULT Initialize(const wstring& _name, wstring _filePath, void* _desc);
	void OnDestroy() override;

public:
	void Bind(const _fmatrix _world, const _cmatrix _view, const _cmatrix _projection, const _uint _boneCount) const;

public:
	class CTexture* Get_Texture(_int _index) const;

public:
	void Set_Shader(CShader* _shader);
	void Set_DiffuseColor(const ColorValue& _color);
	void Set_Texture(CTexture* _texture, _int _index);

private:
	HRESULT Create_ConstantBuffer();

	void Bind_Texture() const;

private:
	CShader* m_pShader;

	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pCameraBuffer;
	ID3D11Buffer* m_pMaterialBuffer;

	vector<class CTexture*> m_vTextureList;
	ColorValue m_vDiffuseColor;
};

NS_END

