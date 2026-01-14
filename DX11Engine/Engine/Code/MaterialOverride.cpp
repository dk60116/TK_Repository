#include "epch.h"
#include "Material.h"

void CMaterial::Bind_CameraWithShader(CShader* _shader, const _float3 _camPos, const _fmatrix _view, const _cmatrix _projection, const _uint _boneCount)
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	if (_shader)
		_shader->Bind();
	else if (m_pShader)
		m_pShader->Bind();

	Bind_Texture();

	// b1: PerCamera
	CameraCB camCB = {};
	camCB.camPos = _camPos;
	camCB.view = XMMatrixTranspose(_view);
	camCB.proj = XMMatrixTranspose(_projection);
	_matrix viewInv = XMMatrixInverse(nullptr, _view);
	camCB.viewInv = XMMatrixTranspose(viewInv);
	context->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &camCB, 0, 0);
	context->PSSetConstantBuffers(1, 1, &m_pCameraBuffer);
	context->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	// b2: PerMaterial
	MaterialCB mat = {};

	mat.baseColor = m_vBaseColor;
	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);
	mat.boneCount = _boneCount;

	context->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	context->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
	context->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

	if (m_vCustomBufferByteList.size() > 0)
		Bind_CustomValues();
}
