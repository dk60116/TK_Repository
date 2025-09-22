#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CParticleSystem final : public CComponent
{
    friend class CGameObject;

    enum class ParticleShape { Sphere, Box };

    typedef struct ParticleInformation
    {
        _bool loop = true;
        _bool playOnAwake = true;
        _uint shape = 0;
        _uint maxCount = 1000;

        _float lifeTime = 5.f;
        _float startDelay = 1.f;
        _float startLifeTime = 1.f;
        vector3 startDistance = vector3::zero();
        vector3 startVelocity = vector3::one();
        vector3 startRotation = vector3::zero();
        _bool startSize3D = false;
        _bool size3D = false;
        _float startSizeMin = 0.05f;
        _float startSizeMax = 0.1f;
        vector3 startSizeMin3D = vector3::one() * 0.1f;
        vector3 startSizeMax3D = vector3::one() * 0.2f;
        ColorValue startColor = ColorValue::blue();
        ColorValue endColor = ColorValue::green();
        _bool useGravity = false;

    }PARTICLEINFO;

    typedef struct ParticleSystemPart
    {
        wstring buffer = L"Quad";
        wstring material = L"ParticleMaterial";
        wstring texture = L"Circle";
        PARTICLEINFO info = {};
    }PARTICLEPART;

    typedef struct ParticleDescription
    {
        _uint count = 1;
        vector<ParticleSystemPart> particles = {};
    } PARTICLEDESC;

private:
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

public:
    void Play();

private:
    CMeshBuffer* CreateInstanceBuffer(CMeshBuffer* _origin, _uint _count, D3D11_USAGE _usage);
    void SetStartValue(CMeshBuffer& _buffer, const _uint _count);

private:
    vector<_bool> m_vPlaying;
    vector<CMeshRenderer*> m_vRenderers;
    PARTICLEDESC m_sDescription;
    vector<CMeshBuffer*> m_vInstanceBuffers;
    vector<_float> m_vWaitTimes, m_vCurrentTimes;
};

NS_END
