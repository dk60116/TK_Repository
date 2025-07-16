#include "epch.h"
#include "SkinnedMeshRenderer.h"
#include "EditorCamera.h"

CSkinnedMeshRenderer::CSkinnedMeshRenderer()
	: CRenderer{}
	, m_pMeshBuffer(nullptr)
	, m_vBones({})
	, m_pRootBone(nullptr)
	, m_pBoneMatrixBuffer(nullptr)
{
	m_strName = L"Skinned Mesh Renderer";
}

CSkinnedMeshRenderer::~CSkinnedMeshRenderer()
{
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::Create()
{
	return new CSkinnedMeshRenderer();
}

HRESULT CSkinnedMeshRenderer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	// 본 행렬 상수 버퍼 생성 (최대 본 갯수 = 128 가정)
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMMATRIX) * 128;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pBoneMatrixBuffer)))
		return E_FAIL;

	return S_OK;
}

void CSkinnedMeshRenderer::OnPreCull()
{
}

void CSkinnedMeshRenderer::OnPreRender()
{
}

void CSkinnedMeshRenderer::Render_Editor()
{
	m_pContext->OMSetDepthStencilState(CSceneManager::GetInstance().Get_CrtScene()->Get_MeshStencillState(), 0);
	Render_WithCamera(CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera());
	Render_Outline(CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera());
}

void CSkinnedMeshRenderer::Render()
{
	CSceneManager::GetInstance().Get_CrtScene()->Get_Camera()->Add_RenderTarget_Mesh(this);
}

void CSkinnedMeshRenderer::OnPostRender()
{
}

void CSkinnedMeshRenderer::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pBoneMatrixBuffer);
	Safe_Release(m_pMeshBuffer);

	for (TRAVERSAL_ITER(m_vBones, it))
		Safe_Release(*it);
	m_vBones.clear();
}

const _uint CSkinnedMeshRenderer::Get_BoneCount() const
{
	return static_cast<_uint>(m_vBones.size());
}

const wstring CSkinnedMeshRenderer::Get_BoneName(const _uint _index) const
{
	return m_vBones[_index]->Get_GameObject()->Get_ObjectName();
}

CTransform* CSkinnedMeshRenderer::Get_BoneTransform(const _uint _index) const
{
	return m_vBones[_index];
}

const _float4x4& CSkinnedMeshRenderer::Get_BoneOffsetMatrix(const _uint _index) const
{
	return m_pMeshBuffer->Get_BoneOffsetMatrix(_index);
}

void CSkinnedMeshRenderer::CreateBoneHierachy(const aiNode* _node, CTransform* _parent)
{
	CGameObject* boneGO = m_pGameObject->Get_Scene()->Add_GameObject(CEngineString::StringToWString(_node->mName.C_Str()));

	CTransform* boneTransform = boneGO->Get_Transform();

	if (_parent)
		boneTransform->SetParent(_parent);

	aiVector3D scaling, position;
	aiQuaternion rotation;
	_node->mTransformation.Decompose(scaling, rotation, position);

	boneTransform->Set_LocalPosition
	(
		position.x,
		position.y,
		position.z
	);
	
	boneTransform->Set_LocalScale(scaling.x, scaling.y, scaling.z);
	boneTransform->Set_LocalQuaternion(quaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	wstring nodeName = CEngineString::StringToWString(_node->mName.C_Str());

	auto it = find(m_pMeshBuffer->m_vBoneNames.begin(), m_pMeshBuffer->m_vBoneNames.end(), nodeName);
	if (it != m_pMeshBuffer->m_vBoneNames.end())
	{
		// 순서 보존을 위해 인덱스를 구해서 정확한 위치에 삽입
		size_t index = distance(m_pMeshBuffer->m_vBoneNames.begin(), it);
		if (m_vBones.size() <= index)
			m_vBones.resize(index + 1, nullptr);

		m_vBones[index] = boneTransform;
		boneTransform->AddRef();
	}

	for (_uint i = 0; i < _node->mNumChildren; ++i)
		CreateBoneHierachy(_node->mChildren[i], boneTransform);
}

void CSkinnedMeshRenderer::Render_WithCamera(CCamera* _cam)
{
	if (!_cam)
	{
		CDebug::LogError("MeshRenderer: No Camera assigned.");
		return;
	}

	if (!m_pMaterial)
	{
		CDebug::LogError(L"Skinned MeshRenderer: No material assigned: " + m_pGameObject->Get_ObjectNameID());
		return;
	}

	if (!m_pMeshBuffer)
	{
		CDebug::LogError(L"Skinned MeshRenderer: No MeshBuffer assigned:" + m_pGameObject->Get_ObjectNameID());
		return;
	}

	// 1) 월드, 뷰, 프로젝션 매트릭스
	vector3 cPos = _cam->Get_Transform()->Get_Position();
	_float3 camPos = cPos.toFloat3();
	_matrix matWorld = m_pGameObject->Get_Transform()->Get_WorldMatrix();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();

	// 2) 본 행렬 계산
	//    m_vBones: 각 본 Transform
	//    m_pMeshBuffer->m_vBoneOffsetMatrices: 역 바인드포즈 행렬
	_matrix boneMatrices[128] = {};

	for (_uint i = 0; i < m_vBones.size(); ++i)
	{
		if (m_vBones[i])
		{
			// 현재 본의 월드 행렬
			_matrix boneWorld = m_vBones[i]->Get_WorldMatrix();

			// 역 바인드 포즈
			_matrix invBindPose = XMLoadFloat4x4(&m_pMeshBuffer->m_vBoneOffsetMatrices[i]);

			if (m_pGameObject) // 메시 Transform
			{
				_matrix meshWorldInv = XMMatrixInverse(nullptr, m_pGameObject->Get_Transform()->Get_WorldMatrix());
				boneWorld = boneWorld * meshWorldInv;
			}

			// 최종 본 행렬
			boneMatrices[i] = XMMatrixTranspose(invBindPose * boneWorld);
		}
		else
		{
			boneMatrices[i] = XMMatrixIdentity();
		}
	}

	// 3) 본 매트릭스 버퍼에 업로드
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	if (SUCCEEDED(m_pContext->Map(m_pBoneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes)))
	{
		memcpy(mappedRes.pData, boneMatrices, sizeof(XMMATRIX) * m_vBones.size());
		m_pContext->Unmap(m_pBoneMatrixBuffer, 0);
	}

	// 4) 머티리얼 바인딩
	m_pMaterial->Bind(matWorld, camPos, matView, matProj, static_cast<_uint>(m_vBones.size()));

	// 5) 본 상수 버퍼 바인딩 (b3 슬롯)
	m_pContext->VSSetConstantBuffers(3, 1, &m_pBoneMatrixBuffer);

	// 6) 메시 렌더링
	m_pMeshBuffer->Render();
}

void CSkinnedMeshRenderer::Render_Outline(CCamera* _cam)
{
	return;

	if (!_cam)
		return;

	D3D11_RASTERIZER_DESC rtDesc = {};
	rtDesc.FillMode = D3D11_FILL_SOLID;
	rtDesc.CullMode = D3D11_CULL_FRONT;
	rtDesc.DepthClipEnable = TRUE;
	rtDesc.DepthBias = 0;
	rtDesc.SlopeScaledDepthBias = 0;
	rtDesc.DepthBiasClamp = 0;
	rtDesc.MultisampleEnable = FALSE;
	rtDesc.AntialiasedLineEnable = FALSE;
	rtDesc.ScissorEnable = FALSE;

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	ID3D11RasterizerState* outlineRasterizer;
	m_pDevice->CreateRasterizerState(&rtDesc, &outlineRasterizer);

	ID3D11DepthStencilState* outlineStencil;
	m_pDevice->CreateDepthStencilState(&dsDesc, &outlineStencil);

	// 월드, 뷰, 프로젝션
	_matrix matWorld = m_pGameObject->Get_Transform()->Get_WorldMatrix();

	// 스케일업 (조금 크게)
	_matrix scale = XMMatrixScaling(1.03f, 1.03f, 1.03f);
	matWorld = scale * matWorld;

	vector3 cPos = _cam->Get_Transform()->Get_Position();
	_float3 camPos = cPos.toFloat3();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();

	m_pContext->OMSetDepthStencilState(CGraphicDevice::GetInstance().Get_DepthStencil_NoWrite(), 0);

	m_pContext->RSSetState(CGraphicDevice::GetInstance().Get_Rasterizer_CullFront());

	// 아웃라인 머티리얼 바인딩 (단색 셰이더)
	
	//if (m_pOutlineMat)
	//{
	//	m_pOutlineMat->Set_DiffuseColor(ColorValue::red());
	//	m_pOutlineMat->Bind(matWorld, camPos, matView, matProj, static_cast<_uint>(m_vBones.size()));

	//	// 본 상수 버퍼 바인딩
	//	m_pContext->VSSetConstantBuffers(3, 1, &m_pBoneMatrixBuffer);

	//	// 메시 렌더링
	//	m_pMeshBuffer->Render();
	//}

	// 5) 상태 복원
	m_pContext->OMSetDepthStencilState(nullptr, 0);
	m_pContext->RSSetState(nullptr);
}

void CSkinnedMeshRenderer::Set_Mesh(CSkinnedMeshBuffer* _mesh)
{
	if (_mesh == m_pMeshBuffer)
		return;

	Safe_Release(m_pMeshBuffer);

	m_pMeshBuffer = _mesh;
	
	if (!m_pMeshBuffer)
	{
		CDebug::LogError("Skinned MeshRenderer: No MeshBuffer");
		return;
	}

	m_pMeshBuffer->AddRef();

	if (m_pMeshBuffer->m_pAssimpScene)
	{
		// RootNode를 어디서 시작할지 결정
		const aiNode* rootBoneNode = m_pMeshBuffer->m_pAssimpScene->mRootNode;
		if (rootBoneNode)
		{
			// MeshRenderer GameObject에 RootBone을 붙임
			m_vBones.clear();
			CreateBoneHierachy(rootBoneNode, m_pGameObject->Get_Transform());
		}
	}
}
