#include "epch.h"
#include "MeshRenderer.h"

void CMeshRenderer::Render_GBuffer(CCamera* _cam, CShader* _gbufferShader)
{
	if (!_cam)
	{
		CDebug::LogError(L"MeshRenderer: No Camera assigned." + m_pGameObject->Get_ObjectNameID());
		return;
	}

	if (!m_pMeshFilter)
	{
		CDebug::LogError(L"MeshRenderer: No MeshFilter assigned:" + m_pGameObject->Get_ObjectNameID());
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

	vector3 cPos = _cam->Get_Transform()->Get_Position();
	_float3 camPos = cPos.toFloat3();
	_matrix matWorld = Get_Transform()->Get_WorldMatrix();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();

	m_pMaterial->Bind_Matrix(matWorld);
	m_pMaterial->Bind_CameraWithShader(_gbufferShader, camPos, matView, matProj, 0);

	auto& inst = pBuffer->Get_InstancingDesc();

	if (inst.dcapacity < 1)
	{
		if (FAILED(pBuffer->CreateInstanceBuffer(1, D3D11_USAGE_DYNAMIC)))
		{
			CDebug::LogError(L"MeshRenderer: failed to create instance buffer for " + m_pGameObject->Get_ObjectNameID());
		}
	}

	if (inst.count == 0 || inst.dcapacity == 0)
		Bind_InstanceData(matWorld, pBuffer);

	pBuffer->Render();

	ResetShaderResources();
}
