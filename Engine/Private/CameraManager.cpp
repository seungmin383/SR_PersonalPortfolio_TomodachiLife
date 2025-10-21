#include "CameraManager.h"
#include "Camera.h"
#include "ShaderCom.h"

CCameraManager::CCameraManager()
{
}

HRESULT CCameraManager::Add_Camera(const _wstring CameraTag, CCamera* pCamera)
{
	auto iter = m_Cameras.find(CameraTag);

	if (iter != m_Cameras.end())
		return E_FAIL;

	m_Cameras.emplace(CameraTag, pCamera);
	return S_OK;
}

int32_t CCameraManager::Update_Priority(_float fDT)
{
	for (auto& pair : m_Cameras)
		pair.second->Update_Priority(fDT);

	return 0;
}
int32_t CCameraManager::Update(_float fDT)
{
	for (auto& pair : m_Cameras)
		pair.second->Update(fDT);

	return 0;
}
void CCameraManager::Update_Late(_float fDT)
{
	for (auto& pair : m_Cameras)
		pair.second->Update_Late(fDT);

}

HRESULT CCameraManager::Bind_Matrix(
	CShaderCom* pShader, 
	const _char* ConstantName, 
	const _wstring CameraTag, 
	D3DTS eMatrix)
{
	auto iter = m_Cameras.find(CameraTag);

	if (iter == m_Cameras.end())
		return E_FAIL;
	switch (eMatrix)
	{
	case Engine::D3DTS::VIEW:
		pShader->Bind_Matrix(ConstantName, &(iter->second->Get_ViewMatrix()));
		break;
	case Engine::D3DTS::PROJ:
		pShader->Bind_Matrix(ConstantName, &(iter->second->Get_ProjMatrix()));
		break;

	case Engine::D3DTS::END:
		return E_FAIL;
	default:
		return E_FAIL;
	}

	return S_OK;
}

CCameraManager* CCameraManager::Create()
{
	return new CCameraManager;
}

void CCameraManager::Free()
{
	for (auto& pair : m_Cameras)
		Safe_Release(pair.second);
	m_Cameras.clear();
}
