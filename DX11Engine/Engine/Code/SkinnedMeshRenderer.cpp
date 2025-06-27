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

	// 본 행렬용 ConstantBuffer 생성
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(_matrix) * 256; // 최대 본 256개
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (FAILED(CGraphicDevice::GetInstance().Get_Device()->CreateBuffer(&desc, nullptr, &m_pBoneMatrixBuffer)))
	{
		CDebug::LogError("SkinnedMeshRenderer: BoneMatrixBuffer Create Failed");
		return E_FAIL;
	}

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

void CSkinnedMeshRenderer::CreateSkinnedMeshBuffer(const string& _filePath, const _float _scaleFactor)
{
	Safe_Release(m_pMeshBuffer);

	m_pMeshBuffer = CSkinnedMeshBuffer::Create();
	m_pMeshBuffer->AddRef();

	if (!m_pMeshBuffer)
	{
		CDebug::LogError("SkinnedMeshRenderer: SkinnedMeshBuffer 생성 실패");
		return;
	}

	if (FAILED(m_pMeshBuffer->Initialize(_filePath , _scaleFactor)))
	{
		CDebug::LogError("SkinnedMeshRenderer: SkinnedMeshBuffer 초기화 실패");
		Safe_Release(m_pMeshBuffer);
		return;
	}
}

void CSkinnedMeshRenderer::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pBoneMatrixBuffer);
	Safe_Release(m_pMeshBuffer);
}

void CSkinnedMeshRenderer::Render_WithCamera(CCamera* _cam)
{
	if (!_cam || !m_pMeshBuffer || !m_pMaterial)
		return;

	// 본 행렬 업데이트
	using BoneMatrix = XMFLOAT4X4A; // 반드시 16바이트 정렬 보장

	vector<BoneMatrix> boneMatrices;
	boneMatrices.resize(m_vBones.size());

	for (UINT i = 0; i < m_vBones.size(); ++i)
	{
		const _matrix& bindPoseInv = m_pMeshBuffer->Get_BindPoseMatricesInverse(i);
		_matrix boneWorld = m_vBones[i]->Get_WorldMatrix();
		_matrix final = bindPoseInv * boneWorld;

		// Transpose 후 저장
		XMStoreFloat4x4A(&boneMatrices[i], XMMatrixTranspose(final));
	}

	// GPU에 업로드 (메모리 정렬 안전)
	if (!boneMatrices.empty())
	{
		CGraphicDevice::GetInstance().Get_Context()->UpdateSubresource
		(
			m_pBoneMatrixBuffer,
			0, nullptr,
			boneMatrices.data(),
			0, 0
		);
	}

	// Material 바인딩
	m_pMaterial->Bind(
		Get_Transform()->Get_WorldMatrix(),
		_cam->Get_ViewMatrix(),
		_cam->Get_ProjectionMatrix()
	);

	// Bone Matrix 버퍼 바인딩 (b3)
	ID3D11DeviceContext* ctx = CGraphicDevice::GetInstance().Get_Context();
	ctx->VSSetConstantBuffers(3, 1, &m_pBoneMatrixBuffer);

	// Mesh 렌더링
	m_pMeshBuffer->Render();
}

void CSkinnedMeshRenderer::SetMesh(CMeshBuffer* _mesh)
{
	Safe_Release(m_pMeshBuffer);

	m_pMeshBuffer = dynamic_cast<CSkinnedMeshBuffer*>(_mesh);

	if (m_pMeshBuffer)
		m_pMeshBuffer->AddRef();
	else
	{
		m_pMeshBuffer = nullptr;
		return;
	}
}

void CSkinnedMeshRenderer::SetBones(const vector<CTransform*>& _bones)
{
	m_vBones = _bones;
}

void CSkinnedMeshRenderer::SetRootBone(CTransform* _root)
{
	m_pRootBone = _root;
}
