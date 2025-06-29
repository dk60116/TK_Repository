#include "epch.h"
#include "SkinnedMeshRenderer.h"

CSkinnedMeshRenderer::CSkinnedMeshRenderer()
	: m_pMeshBuffer(nullptr)
	, m_vBones({})
	, m_pRootBone(nullptr)
	, m_pBoneMatrixBuffer(nullptr)
	, m_iCBSize(0)
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

	m_iCBSize = desc.ByteWidth;

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

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile
	(
		_filePath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs
	);

	if (scene && scene->mRootNode)
	{
		// 1) 계층 생성
		m_pRootBone = CreateBoneHierarchy(scene->mRootNode, nullptr);

		// 2) 본 이름 리스트 얻기
		const auto& boneNames = m_pMeshBuffer->Get_BoneNames();

		// 3) Flat하게 본 Transform만 수집
		m_vBones.clear();
		CollectBonesRecursive(m_pRootBone, boneNames);
	}
}

CTransform* CSkinnedMeshRenderer::CreateBoneHierarchy(const aiNode* _node, CTransform* _parent)
{
	const aiString& name = _node->mName;
	string nameUTF8 = name.C_Str();

	wstring wname = CEngineString::StringToWString(nameUTF8);

	CGameObject* newObj = m_pGameObject->Get_Scene()->Add_GameObject(wname);

	CTransform* bone = newObj->AddComponent<CTransform>();
	
	if (_parent)
		bone->Set_Parent(_parent);

	for (UINT i = 0; i < _node->mNumChildren; ++i)
		CreateBoneHierarchy(_node->mChildren[i], bone);

	return bone;
}

void CSkinnedMeshRenderer::CollectBonesRecursive(CTransform* _node, const vector<string>& _boneNames)
{
	if (!_node)
		return;

	string nodeName = CEngineString::WStringToString(_node->Get_GameObject()->Get_ObjectName());

	if (find(_boneNames.begin(), _boneNames.end(), nodeName) != _boneNames.end())
		m_vBones.push_back(_node);

	for (CTransform* child : _node->Get_ChldLIst())
		CollectBonesRecursive(child, _boneNames);
}

void CSkinnedMeshRenderer::EnsureBoneCB(const UINT _boneCount)
{
	UINT bytes = UINT(sizeof(_matrix) * _boneCount);
	bytes = (bytes + 15) & ~15U;

	if (m_pBoneMatrixBuffer && m_iCBSize == 0)
	{
		D3D11_BUFFER_DESC d{};
		m_pBoneMatrixBuffer->GetDesc(&d);
		m_iCBSize = d.ByteWidth;
	}

	if (m_pBoneMatrixBuffer && m_iCBSize >= bytes)
		return;

	Safe_Release(m_pBoneMatrixBuffer);

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = bytes;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CGraphicDevice::GetInstance().Get_Device()->CreateBuffer(&desc, nullptr, &m_pBoneMatrixBuffer);

	m_iCBSize = bytes;
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

	const auto& boneNames = m_pMeshBuffer->Get_BoneNames();

	EnsureBoneCB(static_cast<UINT>(m_vBones.size()));

	// 16바이트 정렬 문제가 없는 안전한 타입으로 변경
	using BoneMatrix = XMFLOAT4X4;

	vector<BoneMatrix> boneMatrices;
	boneMatrices.resize(m_vBones.size());

	for (UINT i = 0; i < m_vBones.size(); ++i)
	{
		// 바인드포즈 역행렬과 현재 월드 행렬을 곱해서 스킨 매트릭스 계산
		const _matrix& bindPoseInv = m_pMeshBuffer->Get_BindPoseMatricesInverse(i);
		_matrix boneWorld = m_vBones[i]->Get_WorldMatrix();
		_matrix final = bindPoseInv * boneWorld;

		// Transpose 후 안전하게 저장
		XMStoreFloat4x4(&boneMatrices[i], XMMatrixTranspose(final));
	}

	// GPU에 업로드
	if (!boneMatrices.empty())
	{
		CGraphicDevice::GetInstance().Get_Context()->UpdateSubresource(
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

	// 본 매트릭스 버퍼 바인딩 (b3)
	ID3D11DeviceContext* ctx = CGraphicDevice::GetInstance().Get_Context();
	ctx->VSSetConstantBuffers(3, 1, &m_pBoneMatrixBuffer);

	// 메시 렌더링
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

CTransform* CSkinnedMeshRenderer::FindBone(const wstring& _boneName)
{
	for (CTransform* bone : m_vBones)
	{
		if (bone->Get_GameObject()->Get_ObjectName() == _boneName)
			return bone;
	}

	return nullptr;
}
