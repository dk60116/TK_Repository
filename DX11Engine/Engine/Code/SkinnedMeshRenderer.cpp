#include "epch.h"
#include "SkinnedMeshRenderer.h"

CSkinnedMeshRenderer::CSkinnedMeshRenderer()
	: m_pMeshBuffer(nullptr)
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

	if (FAILED(CGraphicDevice::GetInstance().Get_Device()->CreateBuffer(&desc, nullptr, &m_pBoneMatrixBuffer)))
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
	Render_WithCamera(CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera());
}

void CSkinnedMeshRenderer::Render()
{
	Render_WithCamera(CSceneManager::GetInstance().Get_CrtScene()->Get_Camera());
}

void CSkinnedMeshRenderer::OnPostRender()
{
}

void CSkinnedMeshRenderer::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pBoneMatrixBuffer);
	Safe_Release(m_pMeshBuffer);
}

void CSkinnedMeshRenderer::CreateBoneHierachy(const aiNode* _node, CTransform* _parent)
{
	CGameObject* boneGO = m_pGameObject->Get_Scene()->Add_GameObject(CEngineString::StringToWString(_node->mName.C_Str()));

	if (_parent)
		boneGO->Get_Transform()->Set_Parent(_parent);

	for (UINT i = 0; i < _node->mNumChildren; ++i)
		CreateBoneHierachy(_node->mChildren[i], boneGO->Get_Transform());
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
		CDebug::LogError("MeshRenderer: No material assigned.");
		return;
	}

	if (!m_pMeshBuffer)
	{
		CDebug::LogError("SkinnedMeshRenderer: No MeshBuffer assigned.");
		return;
	}

	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	// 1) 월드, 뷰, 프로젝션 매트릭스
	_matrix matWorld = m_pGameObject->Get_Transform()->Get_WorldMatrix();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();

	// 2) 본 행렬 계산
	//    m_vBones: 각 본 Transform
	//    m_pMeshBuffer->m_vBoneOffsetMatrices: 역 바인드포즈 행렬
	XMMATRIX boneMatrices[128] = {};

	for (UINT i = 0; i < m_vBones.size(); ++i)
	{
		if (m_vBones[i])
		{
			// 현재 본의 월드 행렬
			_matrix boneWorld = m_vBones[i]->Get_WorldMatrix();

			// 역 바인드 포즈
			_matrix invBindPose = m_pMeshBuffer->m_vBoneOffsetMatrices[i];

			// 최종 본 행렬: BoneWorld * InverseBindPose
			boneMatrices[i] = XMMatrixTranspose(invBindPose * boneWorld);
		}
		else
		{
			boneMatrices[i] = XMMatrixIdentity();
		}
	}

	// 3) 본 매트릭스 버퍼에 업로드
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	if (SUCCEEDED(context->Map(m_pBoneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes)))
	{
		memcpy(mappedRes.pData, boneMatrices, sizeof(XMMATRIX) * m_vBones.size());
		context->Unmap(m_pBoneMatrixBuffer, 0);
	}

	// 4) 머티리얼 바인딩
	m_pMaterial->Bind(matWorld, matView, matProj);

	// 5) 본 상수 버퍼 바인딩 (b1 슬롯)
	context->VSSetConstantBuffers(1, 1, &m_pBoneMatrixBuffer);

	// 6) 메시 렌더링
	m_pMeshBuffer->Render();
}

void CSkinnedMeshRenderer::Set_Mesh(CMeshBuffer* _mesh)
{
	m_pMeshBuffer = dynamic_cast<CSkinnedMeshBuffer*>(_mesh);
	
	if (!m_pMeshBuffer)
	{
		CDebug::LogError("Skinned MeshRenderer: No MeshBuffer");
		return;
	}

	if (m_pMeshBuffer->m_pAssimpScene)
	{
		// RootNode를 어디서 시작할지 결정
		const aiNode* rootBoneNode = m_pMeshBuffer->m_pAssimpScene->mRootNode->FindNode("Armature");
		if (rootBoneNode)
		{
			// MeshRenderer GameObject에 RootBone을 붙임
			CreateBoneHierachy(rootBoneNode, m_pGameObject->Get_Transform());
		}
	}

	// m_vBones 초기화
	m_vBones.clear();

	// 본 이름 리스트를 순회
	for (const auto& boneName : m_pMeshBuffer->m_vBoneNames)
	{
		// 현재 오브젝트 하위에서 본 이름에 맞는 Transform 찾아서 연결
		CTransform* pBone = m_pGameObject->Get_Transform()->Find_Child(boneName);
		m_vBones.push_back(pBone);
	}
}
