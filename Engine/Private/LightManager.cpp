#include "LightManager.h"
#include "Light.h"

CLightManager::CLightManager()
{
}

const LIGHT_DESC* CLightManager::Get_LightDesc(uint32_t iIndex)
{
	auto iter = m_Lights.begin();

	for (uint32_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLightManager::Add_Light(const LIGHT_DESC& eLight)
{
	CLight* pLight = CLight::Create(eLight);
	if (nullptr == pLight)
		return E_FAIL;
	
	m_Lights.push_back(pLight);
	return S_OK;
}

CLightManager* CLightManager::Create()
{
	return new CLightManager;
}

void CLightManager::Free()
{
	CBase::Free();
}
