#include "epch.h"
#include "MeshRenderer.h"

CMeshRenderer::CMeshRenderer()
	: CRenderer{}
	, m_pMeshFilter(nullptr)
{
	m_strName = L"Mesh Renderer";
}

CMeshRenderer::~CMeshRenderer()
{
}

CMeshRenderer* CMeshRenderer::Create()
{
	return new CMeshRenderer();
}

CComponent* CMeshRenderer::Clone() const
{
	CMeshRenderer* clone = new CMeshRenderer();

	return clone;
}

HRESULT CMeshRenderer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (!m_pMeshFilter)
	{
		m_pMeshFilter = m_pGameObject->AddComponent<CMeshFilter>();

		if (m_pMeshFilter)
			m_pMeshFilter->AddRef();
	}

	return S_OK;
}

void CMeshRenderer::OnPreCull()
{
}

void CMeshRenderer::OnPreRender()
{
}

void CMeshRenderer::Render_Editor()
{
	m_pContext->OMSetDepthStencilState(CSceneManager::Get_CrtScene()->Get_MeshStencillState(), 0);
	Render_WithCamera(CSceneManager::Get_CrtScene()->Get_EditorCamera());
}

void CMeshRenderer::Render()
{
	_uint _layer = m_pGameObject->GetLayer();

	if (_layer != CSceneManager::NameToLayer(L"NaviMesh_Walkable") && _layer != CSceneManager::NameToLayer(L"NaviMesh_WalkUnable"))
		CSceneManager::Get_CrtScene()->Get_Camera()->Add_RenderTarget_Mesh(this);
}

void CMeshRenderer::OnPostRender()
{
}

void CMeshRenderer::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pMeshFilter);
}

void CMeshRenderer::Render_WithCamera(CCamera* _cam)
{
    if (!_cam)
    {
        CDebug::LogError(L"MeshRenderer: No Camera assigned." + m_pGameObject->Get_ObjectNameID());
        return;
    }
    if (!m_pMeshFilter)
    {
        CDebug::LogError(L"MeshRenderer: No MeshFilter assigned: " + m_pGameObject->Get_ObjectNameID());
        return;
    }
    if (!m_pMaterial)
    {
        CDebug::LogError(L"MeshRenderer: No material assigned: " + m_pGameObject->Get_ObjectNameID());
        return;
    }

    CMeshBuffer* pBuffer = m_pMeshFilter->Get_MeshBuffer();
    if (!pBuffer) 
        return;

    // Camera / Matrices
    vector3 cPos = _cam->Get_Transform()->Get_Position();
    _float3 camPos = cPos.toFloat3();
    _matrix matWorld = Get_Transform()->Get_WorldMatrix();
    _matrix matView = _cam->Get_ViewMatrix();
    _matrix matProj = _cam->Get_ProjectionMatrix();

    // PerObject( b0 ), PerCamera( b1 ), PerMaterial( b2 )
    if (m_bUseInstancing)
    {
        // 인스턴싱: 상수버퍼 world는 IDENTITY로 두고, 실제 월드변환은 인스턴스 버퍼로 보냄
        _matrix identity = XMMatrixIdentity();
        m_pMaterial->Bind_Matrix(identity);
    }
    else
    {
        // 일반 렌더: 상수버퍼에 월드 그대로
        m_pMaterial->Bind_Matrix(matWorld);
    }

    // 카메라/머티리얼 바인딩 (PS에도 b1 세팅되도록 구현되어 있음)
    m_pMaterial->Bind_Camera(camPos, matView, matProj, 0);

    // Light
    if (m_pMaterial->IsUseLight())
    {
        list<CLight*> lights = CSceneManager::Get_CrtScene()->Get_LightList();
        const _uint lightCount = static_cast<_uint>(lights.size());

        vector<_matrix> vLightInfos;
        vLightInfos.reserve(lightCount);

        _uint index = 0;
        for (TRAVERSAL_ITER(lights, it))
        {
            if (!(*it)) 
                continue;

            _float4x4 lightInfo = (*it)->To_LightInfo();
            // gLight[0][3][3]에 조명 갯수 넣는 규약 유지
            lightInfo._44 = (index == 0) ? static_cast<_float>(lightCount) : 0.f;

            vLightInfos.push_back(XMLoadFloat4x4(&lightInfo));
            ++index;
        }

        if (!vLightInfos.empty())
            m_pMaterial->Bind_Light(vLightInfos.data(), lightCount);
    }

    // ------- Draw -------
    if (m_bUseInstancing)
    {
        // 인스턴스 목록이 비어있으면 자기 자신만 1개 인스턴스로 그려줌 (이전 동작과 동일)
        const bool hasList = !m_vInstanceWorlds.empty();

        vector<MeshInstanceData> instances;
        if (hasList)
        {
            instances.reserve(m_vInstanceWorlds.size());
            for (const _float4x4& w : m_vInstanceWorlds)
            {
                _matrix m = XMLoadFloat4x4(&w);
                instances.push_back(MatrixToInstanceData(m));
            }
        }
        else
        {
            instances.push_back(MatrixToInstanceData(matWorld));
        }

        if (SUCCEEDED(pBuffer->UpdateInstanceBuffer(instances, true)))
            pBuffer->RenderInstanced(static_cast<_uint>(instances.size()));
    }
    else
        pBuffer->Render();
}

void CMeshRenderer::Render_Outline(CCamera* _cam)
{
}

CMeshFilter* CMeshRenderer::Get_MeshFilter()
{
	return m_pMeshFilter;
}

CMeshBuffer* CMeshRenderer::Get_MeshBuffer()
{
	return m_pMeshFilter->Get_MeshBuffer();
}
