#include "Light.h"
#include "GameInstance.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& LightDesc)
{
	m_LightDesc = LightDesc;
	return S_OK;
}

void CLight::Render_Light(CShaderCom* pShader, CRectBufferCom* pBuffer)
{
	uint32_t iPassIndex = {};

	switch (m_LightDesc.eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		if (FAILED(pShader->Bind_RawValue("g_vLightDir",&m_LightDesc.vDir, sizeof(XMFLOAT4))))
			return;
		iPassIndex = static_cast<uint32_t>(DEFERRED::DIRECTIONAL);
		break;

	case LIGHT_TYPE::POINT:
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float))))
			return;
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPos, sizeof(XMFLOAT4))))
			return;
		iPassIndex = static_cast<uint32_t>(DEFERRED::POINT);
		break;

	default:
		break;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(XMFLOAT4))))
		return;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(XMFLOAT4))))
		return;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(XMFLOAT4))))
		return;

	if (FAILED(pShader->Begin(iPassIndex)))
		return;
	if (FAILED(pBuffer->Bind_Resource()))
		return;
	if (FAILED(pBuffer->Begin_Draw()))
		return;
}

CLight* CLight::Create(const LIGHT_DESC& LightDesc)
{
	CLight* pLight = new CLight;

	if (FAILED(pLight->Initialize(LightDesc)))
	{
		MSG_BOX("ErrCode : Light Create Failed");
		Safe_Release(pLight);
		return nullptr;
	}
	return pLight;
}

void CLight::Free()
{
	CBase::Free();
}
