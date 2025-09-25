#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CParticleSystem final : public CComponent
{
    friend class CGameObject;

public:
    enum class ParticleShape { Sphere, Box };

    typedef struct ParticleInformation
    {
        _bool loop = true;
        _bool playOnAwake = true;
        _uint shape = 0;
        _uint maxCount = 100;

        _float startDelay = 0.1f;
        _float startLifeTime = 1.f;
        vector3 startDistance = vector3::zero();
        vector3 startVelocity = vector3::one();
        _float startSpeed = 0.5f;
        _float endSpeed = 1.f;
        vector3 startRotation = vector3::zero();
        _bool size3D = false;
        vector4 startSizeMin = vector4::one() * 0.1f;
        vector4 startSizeMax = vector4::one() * 0.2f;
        vector4 endSizeMin = vector4::one() * 0.2f;
        vector4 endSizeMax = vector4::one() * 0.4f;
        ColorValue baseColor = ColorValue::white();
        ColorValue startColor = ColorValue(255, 0, 0, 255);
        ColorValue endColor = ColorValue(255, 0, 0, 0);
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
        vector<ParticleSystemPart> particles = { {} };
    } PARTICLEDESC;

private:
    explicit CParticleSystem();
    ~CParticleSystem();

private:
    static CParticleSystem* Create();
    CComponent* Clone() const override;

public:
    HRESULT Initialize(void* _desc) override;
    void OnEnable() override;
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
