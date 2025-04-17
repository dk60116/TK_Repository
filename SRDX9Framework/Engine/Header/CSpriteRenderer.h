#pragma once

#include "CComponent.h"
#include "CRectCol.h"
#include "CMaterial.h"
#include "CTexture.h"

BEGIN(Engine)

struct SpriteRendererOptions
{
    ColorValue colorTint = ColorValue::white();
    _int sortOrder = 0;
    _bool lighting = false;
};

class ENGINE_DLL CSpriteRenderer final
    : public CComponent
{
public:
    explicit CSpriteRenderer();
    ~CSpriteRenderer();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void Render() override;
    void OnDestroy() override;

public:
    void SetTexture(CTexture* _texture);
    void SetTintColor(ColorValue _color);

    SpriteRendererOptions& getOptions() { return m_sOptions; }

private:
    CTexture* m_pTexture;
    CVIBuffer* m_pBuffer;
    RECT m_rcUV;
    CMaterial* m_pMaterial;

    SpriteRendererOptions m_sOptions;
};

END

