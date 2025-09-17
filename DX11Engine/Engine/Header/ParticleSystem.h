#pragma once
#include "Component.h"

class CParticleSystem abstract : public CComponent
{
	friend class CGameObject;

	enum class ParticleShape { Sphere, Box };

	typedef struct ParticleInformation
	{
		_float startDelay = 0.f;
		_float startLifeTime = 1.f;
		vector3 startDirection = vector3::zero();
		_bool startSize3D = false;
		vector3 startSize = vector3::one();
		vector3 startRotation = vector3::zero();
		ColorValue startColor = ColorValue::white();
		_bool useGravity = false;

	}PARTICLEINFO;

	typedef struct ParticleSystemPart
	{
		CMeshBuffer* buffer = nullptr;
		CMaterial* material = nullptr;
		CTexture* texture = nullptr;
		_uint maxCount = 1;
		PARTICLEINFO info = {};
	}PARTICLEPART;

protected:
	explicit CParticleSystem();
	~CParticleSystem();

private:
	static CParticleSystem* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Update() override;
	void Render_Editor() override;
	void Render() override;

	void OnDestroy() override;

protected:
	vector<ParticleSystemPart> m_vParticleList;
};

