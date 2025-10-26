#include "Light.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& eLight)
{
	m_eLightDesc = eLight;
	return S_OK;
}

CLight* CLight::Create(const LIGHT_DESC& eLight)
{
	CLight* pLight = new CLight;

	if (FAILED(pLight->Initialize(eLight)))
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
