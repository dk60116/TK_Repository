#pragma once

#include "CComponent.h"
#include "CRectCol.h"

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
    void SetTexture(LPDIRECT3DTEXTURE9 _texture) { m_pTexture = _texture; }
    void SetTintColor(ColorValue _color);

private:
    LPDIRECT3DTEXTURE9 m_pTexture;
    CVIBuffer* m_pBuffer;
    RECT m_rcUV;
    ColorValue m_sColorTint;
};

END

