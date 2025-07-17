#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMaterial final : public CEngineResource
{
	friend class CResources;

public:
	typedef struct Material
	{
		class CShader* shaderPointer;
		_bool usingRight = true;
	} MATERIALDESC;

private:
	explicit CMaterial();
	CMaterial(const CMaterial& _other);
	virtual ~CMaterial();

private:
	static CMaterial* Create(const wstring _path = L"");
	static CMaterial* Clone(const CMaterial& _other);

private:
	HRESULT Initialize(const wstring& _name, wstring _filePath, void* _desc);
	void OnDestroy() override;

private:
	void BaseInitValues();

public:
	void Bind(const _fmatrix _world, const _float3 _camPos, const _cmatrix _view, const _cmatrix _projection, const _uint _boneCount = 0) const;
	void Bind_Light(_matrix* _lights, const _uint _count);

public:
	const _bool IsUseLight() const;
	class CTexture* Get_Texture(_int _index) const;

public:
	void Set_Shader(CShader* _shader);
	void Set_Texture(CTexture* _texture, _int _index);
	void Set_FloatValue(const wstring _key, const _float _value);
	void Set_IntValue(const wstring _key, const _int _value);
	void Set_VectorValue(const wstring _key, const _float4 _value);

private:
	HRESULT Create_ConstantBuffer();

	void Bind_Texture() const;

private:
	CShader* m_pShader;

	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pCameraBuffer;
	ID3D11Buffer* m_pMaterialBuffer;
	ID3D11Buffer* m_pLightBuffer;

	_bool m_bUseLight;
	vector<class CTexture*> m_vTextureList;
	unordered_map<wstring, _float> m_mFloatValues;
	unordered_map<wstring, _int> m_mIntValues;
	unordered_map<wstring, _float4> m_mVectorValues;
};

NS_END

