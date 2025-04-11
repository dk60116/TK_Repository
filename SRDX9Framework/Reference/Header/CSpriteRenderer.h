#pragma once

#include "CComponent.h"
#include "CRectCol.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CSpriteRenderer 
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

private:
    CTexture* m_pTexture;
    CVIBuffer* m_pBuffer;
    RECT m_rcUV;
    ColorValue m_sColorTint;
};

END

