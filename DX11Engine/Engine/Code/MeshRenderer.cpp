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

HRESULT CMeshRenderer::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
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
	{
		if (!m_pMaterial->IsTransparent())
		{
			if (!m_bNoneCulling)
				CSceneManager::Get_CrtScene()->Get_Camera()->Add_RenderTarget_Mesh(this);
			else
				CSceneManager::Get_CrtScene()->Get_Camera()->Add_RenderTarget_NoneCullMesh(this);
		}
		else
			CSceneManager::Get_CrtScene()->Get_Camera()->Add_RenderTarget_BlendMesh(this);
	}
}

void CMeshRenderer::Render_Gizmo()
{
	if (m_pGameObject != CEditor::Get_SelectedGameObject())
		return;

	if (!m_pGameObject->IsRecursiveActive())
		return;

	if (!m_pMeshFilter)
		return;

	CMeshBuffer* pBuffer = m_pMeshFilter->Get_MeshBuffer();

	if (!pBuffer)
		return;

	CCamera* editorCam = CSceneManager::Get_CrtScene()->Get_EditorCamera();
	if (!editorCam)
		return;

	const D3D11_VIEWPORT* vp = CGraphicDevice::GetInstance().Get_CurrentViewport();
	ImDrawList* dl = ImGui::GetBackgroundDrawList();
	if (!vp || !dl) 
		return;

	// ── 카메라 행렬
	const _matrix view = editorCam->Get_ViewMatrix();
	const _matrix proj = editorCam->Get_ProjectionMatrix();
	const _matrix VP = XMMatrixMultiply(view, proj);

	const wstring name = m_pGameObject->Get_ObjectName();

	// ── 로컬 AABB
	const auto aabb = pBuffer->Get_AABB();
	const vector3& mn = aabb.min;
	const vector3& mx = aabb.max;

	// ── 로컬 꼭짓점 8개
	const vector3 localCorners[8] =
	{
		{mn.x, mn.y, mn.z}, {mx.x, mn.y, mn.z}, {mx.x, mx.y, mn.z}, {mn.x, mx.y, mn.z} ,
		{mn.x, mn.y, mx.z}, {mx.x, mn.y, mx.z}, {mx.x, mx.y, mx.z}, {mn.x, mx.y, mx.z}
	};

	auto WorldToScreen = [&](const vector3& pW, ImVec2& out)->_bool
		{
			const _vector pw = XMVectorSet(pW.x, pW.y, pW.z, 1.0f);
			const _vector clip = XMVector4Transform(pw, VP);
			const _float w = XMVectorGetW(clip);
			
			if (w <= 1e-6f) 
				return false;

			const _float ndcX = XMVectorGetX(clip) / w;
			const _float ndcY = XMVectorGetY(clip) / w;

			const _float sx = (ndcX * 0.5f + 0.5f) * vp->Width + vp->TopLeftX;
			const _float sy = (-ndcY * 0.5f + 0.5f) * vp->Height + vp->TopLeftY;
			out = ImVec2(sx, sy);
			return true;
		};

	auto DrawEdge = [&](const vector3& aW, const vector3& bW, ImU32 color, _float thickness)
		{
			ImVec2 sa, sb;
			if (WorldToScreen(aW, sa) && WorldToScreen(bW, sb))
				dl->AddLine(sa, sb, color, thickness);
		};

	auto DrawBoxWithWorld = [&](const _matrix& W, ImU32 color, _float thickness)
		{
			// 로컬 → 월드
			vector3 wc[8];

			for (int i = 0; i < 8; ++i)
			{
				const _vector v = XMVectorSet(localCorners[i].x, localCorners[i].y, localCorners[i].z, 1.0f);
				const _vector vw = XMVector3TransformCoord(v, W);
				wc[i] = vector3(XMVectorGetX(vw), XMVectorGetY(vw), XMVectorGetZ(vw));
			}

			// 12개 엣지 그리기
			DrawEdge(wc[0], wc[1], color, thickness); DrawEdge(wc[1], wc[2], color, thickness);
			DrawEdge(wc[2], wc[3], color, thickness); DrawEdge(wc[3], wc[0], color, thickness);

			DrawEdge(wc[4], wc[5], color, thickness); DrawEdge(wc[5], wc[6], color, thickness);
			DrawEdge(wc[6], wc[7], color, thickness); DrawEdge(wc[7], wc[4], color, thickness);

			DrawEdge(wc[0], wc[4], color, thickness); DrawEdge(wc[1], wc[5], color, thickness);
			DrawEdge(wc[2], wc[6], color, thickness); DrawEdge(wc[3], wc[7], color, thickness);
		};

	const ImU32 color = IM_COL32(200, 200, 200, 200);
	const _float thickness = 1.5f;

	const _matrix W = Get_Transform()->Get_WorldMatrix();
	const vector3& localScale = Get_Transform()->Get_LocalScale();
	const _float InverseFactor = 1.f / m_fScaleFactor;
	_matrix S = XMMatrixScaling(InverseFactor, InverseFactor, InverseFactor);
	const _matrix WS = XMMatrixMultiply(S, W);

	DrawBoxWithWorld(WS, color, thickness);
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
		CDebug::LogError(L"MeshRenderer: No MeshFilter assigned:" + m_pGameObject->Get_ObjectNameID());
		CDebug::LogError(L"MeshRenderer: No MeshFilter assigned:" + m_pGameObject->Get_ObjectNameID());
		return;
	}

	if (!m_pMaterial)
	{
		CDebug::LogError(L"MeshRenderer: No material assigned: " + m_pGameObject->Get_ObjectNameID());
		return;
	}

	if (m_pMaterial->IsUseLight() && !_cam->IsDeferredGBuffer())
	{
	}
	CMeshBuffer* pBuffer = m_pMeshFilter->Get_MeshBuffer();

	if (!pBuffer)
		return;

	// World / View / Projection 행렬 계산

	vector3 cPos = _cam->Get_Transform()->Get_Position();
	_float3 camPos = cPos.toFloat3();
	_matrix matWorld = Get_Transform()->Get_WorldMatrix();
	_matrix matView = _cam->Get_ViewMatrix();
	_matrix matProj = _cam->Get_ProjectionMatrix();
	
	m_pMaterial->Bind_Matrix(matWorld);

	m_pMaterial->Bind_Camera(camPos, matView, matProj, 0);

	if (m_pMaterial->IsUseLight())
	{
		list<CLight*> lights = CSceneManager::Get_CrtScene()->Get_LightList();
		const _uint lightCount = static_cast<_uint>(lights.size());

		vector<_matrix> vLightInfos = {};

		_uint index = 0;

		for (TRAVERSAL_ITER(lights, it))
		{
			if (!(*it))
				continue;

			_float4x4 lightInfo = (*it)->To_LightInfo();
			
			if (index == 0)
				lightInfo._44 = static_cast<_float>(lights.size());
			else
				lightInfo._44 = 0.f;

			vLightInfos.push_back(XMLoadFloat4x4(&lightInfo));

			++index;
		}

		m_pMaterial->Bind_Light(vLightInfos.data(), static_cast<_uint>(lights.size()));
	}

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

void CMeshRenderer::Render_Outline(CCamera* _cam)
{
}

void CMeshRenderer::Render_Shadow(const _matrix& lightVP, _bool alphaCutout)
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

void CMeshRenderer::Set_ScaleFactor(const _float _value)
{
	m_fScaleFactor = _value;
}
