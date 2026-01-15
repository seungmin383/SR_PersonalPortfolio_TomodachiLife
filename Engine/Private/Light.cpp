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
	//if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_MINUS))
	//{
	//	m_is = FALSE;
	//}
	//else if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_EQUALS))
	//{
	//	m_is = TRUE;
	//}

	//if (m_is && m_LightDesc.eType == LIGHT_TYPE::POINT)
	//{

	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_UP))
	//	{
	//		m_LightDesc.vPos.z += 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_DOWN))
	//	{
	//		m_LightDesc.vPos.z -= 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_RIGHT))
	//	{
	//		m_LightDesc.vPos.x += 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_LEFT))
	//	{
	//		m_LightDesc.vPos.x -= 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_U))
	//	{
	//		m_LightDesc.vPos.y += 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_J))
	//	{
	//		m_LightDesc.vPos.y -= 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_I))
	//	{
	//		m_LightDesc.fRange += 0.1f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_K))
	//	{
	//		m_LightDesc.fRange -= 0.1f;
	//	}
	//	cout << "Pos X : " << m_LightDesc.vPos.x << " Pos Y : " << m_LightDesc.vPos.y << " Pos Z : " << m_LightDesc.vPos.z << " fRange : " << m_LightDesc.fRange << endl;
	//}
	//else if (!m_is && m_LightDesc.eType == LIGHT_TYPE::DIRECTIONAL)
	//{
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_UP))
	//	{
	//		m_LightDesc.vDir.z += 0.01f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_DOWN))
	//	{
	//		m_LightDesc.vDir.z -= 0.01f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_RIGHT))
	//	{
	//		m_LightDesc.vDir.x += 0.01f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_LEFT))
	//	{
	//		m_LightDesc.vDir.x -= 0.01f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_U))
	//	{
	//		m_LightDesc.vDir.y += 0.01f;
	//	}
	//	if (CGameInstance::GetInstance()->Get_DIKeyState(DIK_J))
	//	{
	//		m_LightDesc.vDir.y -= 0.01f;
	//	}
	//	
	//	cout << "Dir X : " << m_LightDesc.vDir.x << " Dir Y : " << m_LightDesc.vDir.y << " Dir Z : " << m_LightDesc.vDir.z << endl;
	//}
	//


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
