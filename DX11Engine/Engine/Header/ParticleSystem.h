#pragma once
#include "Component.h"

class CParticleSystem : public CComponent
{
	friend class CGameObject;

	enum class ParticleShape { Sphere, Box };

	typedef struct ParticleInformation
	{
		_uint shape;
		_uint maxCount = 1;
		_float lifeTime = 1.f;
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
		wstring buffer = {};
		wstring material = {};
		wstring texture = {};
		PARTICLEINFO info = {};
	}PARTICLEPART;

	typedef struct ParticleDescription
	{
		_uint count;
		vector<ParticleSystemPart> particles;
	} PARTICLEDESC; 

protected:
	explicit CParticleSystem();
	~CParticleSystem();

private:
	static CParticleSystem* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Update() override;
	void Render_Editor() override;
	void Render() override;

	void OnDestroy() override;

private:
	CMeshBuffer* CreateInstaceBuffer();

private:
	vector<ParticleSystemPart> m_vParticleList;
};

