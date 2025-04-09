#pragma once

#include "CComponent.h"
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL SpriteRenderer 
    : public CComponent
{
public:
    SpriteRenderer();
    ~SpriteRenderer();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void Render() override;
    void OnDestroy() override;

public:
    void SetTexture(LPDIRECT3DTEXTURE9 _texture) { m_pTexture = _texture; }
    void SetTintColor(ColorValue _color) { m_sColorTint = _color; }

private:
    LPDIRECT3DTEXTURE9 m_pTexture;
    CVIBuffer* m_pBuffer;
    RECT m_rcUV;
    ColorValue m_sColorTint;
};

END

