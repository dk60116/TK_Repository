#pragma once

#include "CComponent.h"
#include "CRectCol.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CMaterial.h"

BEGIN(Engine)

struct SpriteRendererOptions
{
    ColorValue colorTint = ColorValue::white();
    _int sortOrder = 0;
    _bool lighting = true;
};

class ENGINE_DLL CSpriteRenderer final
    : public CComponent
{
private:
    explicit CSpriteRenderer();
    ~CSpriteRenderer();

public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void RenderEditor() override;
    void Render() override;
    void OnDestroy() override;

public:
    static CSpriteRenderer* Create();

private:
    void Render_Final(CCamera* _camera, _bool _editor);

public:
    void SetTexture(CTexture* _texture);
    void SetTintColor(ColorValue _color);

    SpriteRendererOptions& getOptions() { return m_sOptions; }

private:
    CTexture* m_pTexture;
    CRectCol* m_pBuffer;
    RECT m_rcUV;
    CMaterial* m_pMaterial;

    SpriteRendererOptions m_sOptions;
};

END

