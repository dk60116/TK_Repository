#pragma once

#include "CResource.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :
    public CResource
{
public:
    CTexture();
    ~CTexture();

public:
    HRESULT Load(LPDIRECT3DDEVICE9 _device) override;
    void Release() override;

public:
    LPDIRECT3DTEXTURE9 getTexture() const { return m_pTexture; }
    const vector2Int& getSize() { return m_v2Size; }
    bool isValid() const { return m_pTexture != nullptr; }

private:
    LPDIRECT3DTEXTURE9 m_pTexture;
    D3DSURFACE_DESC m_tDesc;
    vector2Int m_v2Size;
};

END

