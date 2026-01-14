#include "epch.h"
#include "SkinnedMeshRenderer.h"

void CSkinnedMeshRenderer::Render_GBuffer(CCamera* _cam, CShader* _gbufferShader)
{
	if (!_cam)
	{
		CDebug::LogError("Skinned MeshRenderer: No Camera assigned.");
		return;
	}

	if (!m_pMaterial)
	{
		CDebug::LogError(L"Skinned MeshRenderer - No material assigned: " + m_pGameObject->Get_ObjectNameID());
		return;
	}

	if (!m_pMeshBuffer)
	{
		CDebug::LogError(L"Skinned MeshRenderer - No MeshBuffer assigned :" + m_pGameObject->Get_ObjectNameID());
		return;
	}

	vector3 cPos = _cam->Get_Transform()->Get_Position();
	_float3 camPos = cPos.toFloat3();
	_matrix matWorld = m_pGameObject->Get_Transform()->Get_WorldMatrix();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();

	_matrix boneMatrices[128] = {};

	for (_uint i = 0; i < static_cast<_uint>(m_vBones.size()); ++i)
	{
		if (m_vBones[i])
		{
			_matrix boneWorld = m_vBones[i]->Get_WorldMatrix();
			_matrix invBindPose = XMLoadFloat4x4(&m_pMeshBuffer->m_vBoneOffsetMatrices[i]);

			if (m_pGameObject)
			{
				_matrix meshWorldInv = XMMatrixInverse(nullptr, m_pGameObject->Get_Transform()->Get_WorldMatrix());
				boneWorld = boneWorld * meshWorldInv;
			}

			boneMatrices[i] = XMMatrixTranspose(invBindPose * boneWorld);
		}
		else
		{
			boneMatrices[i] = XMMatrixIdentity();
		}
	}

	D3D11_MAPPED_SUBRESOURCE mappedRes;
	if (SUCCEEDED(m_pContext->Map(m_pBoneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes)))
	{
		memcpy(mappedRes.pData, boneMatrices, sizeof(_matrix) * m_vBones.size());
		m_pContext->Unmap(m_pBoneMatrixBuffer, 0);
	}

	CShader* gbufferShader = CResources::LoadOnGame<CShader>(L"DeferredGBufferSkinned (Shader)");

	m_pMaterial->Bind_Matrix(matWorld);
	m_pMaterial->Bind_CameraWithShader(gbufferShader, camPos, matView, matProj, static_cast<_uint>(m_vBones.size()));

	m_pContext->VSSetConstantBuffers(3, 1, &m_pBoneMatrixBuffer);

	Bind_InstanceData(XMMatrixIdentity(), m_pMeshBuffer);
	m_pMeshBuffer->Render();

	ResetShaderResources();
}
