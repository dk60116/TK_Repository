#pragma once

#include "CComponent.h"
#include "CRectCol.h"
#include "CMaterial.h"
#include "CTexture.h"

BEGIN(Engine)

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
    const _int& getSortOrder() { return m_iSortOrder; }
    void SetSortOrder(const _int _value) { m_iSortOrder = _value; }

private:
    CTexture* m_pTexture;
    CVIBuffer* m_pBuffer;
    RECT m_rcUV;
    ColorValue m_sColorTint;
    _int m_iSortOrder;
    CMaterial* m_pMaterial;
};

END

