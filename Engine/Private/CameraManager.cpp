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

HRESULT CCameraManager::Bind_Matrix(CShaderCom* pShader, const _char* ConstantName, const _wstring CameraTag, D3DTS eMatrix)
{
	auto iter = m_Cameras.find(CameraTag);

	if (iter == m_Cameras.end())
		return E_FAIL;

	XMFLOAT4X4 tmpMat;

	switch (eMatrix)
	{
	case Engine::D3DTS::VIEW:
		XMStoreFloat4x4(&tmpMat, iter->second->Get_ViewMatrix());
		if(FAILED(pShader->Bind_Matrix(ConstantName, &(tmpMat))))
			return E_FAIL;
		break;

	case Engine::D3DTS::PROJ:
		XMStoreFloat4x4(&tmpMat, iter->second->Get_ProjMatrix());
		if (FAILED(pShader->Bind_Matrix(ConstantName, &(tmpMat))))
			return E_FAIL;
		break;

	case Engine::D3DTS::END:
		return E_FAIL;
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCameraManager::Bind_Matrix_Inv(CShaderCom* pShader, const _char* ConstantName, const _wstring CameraTag, D3DTS eMatrix)
{
	auto iter = m_Cameras.find(CameraTag);

	if (iter == m_Cameras.end())
		return E_FAIL;

	XMFLOAT4X4 tmpMat;

	switch (eMatrix)
	{
	case Engine::D3DTS::VIEW:
		XMStoreFloat4x4(&tmpMat, XMMatrixInverse(nullptr, iter->second->Get_ViewMatrix()));
		if (FAILED(pShader->Bind_Matrix(ConstantName, &(tmpMat))))
			return E_FAIL;
		break;

	case Engine::D3DTS::PROJ:
		XMStoreFloat4x4(&tmpMat, XMMatrixInverse(nullptr, iter->second->Get_ViewMatrix()));
		if (FAILED(pShader->Bind_Matrix(ConstantName, &(tmpMat))))
			return E_FAIL;
		break;

	case Engine::D3DTS::END:
		return E_FAIL;
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCameraManager::Bind_Camera_Pos(
	CShaderCom* pShader, 
	const _char* pConstantName,
	const _wstring CameraTag)
{
	auto iter = m_Cameras.find(CameraTag);

	if (iter == m_Cameras.end())
		return E_FAIL;

	XMVECTOR tmpVec = iter->second->Get_WorldMatrix().r[3];
	XMFLOAT4 tmp;
	XMStoreFloat4(&tmp, XMVectorSetW(tmpVec, 1.f));

	if (FAILED(pShader->Bind_RawValue(pConstantName, &tmp, sizeof(XMFLOAT4))))
		return E_FAIL;

	return S_OK;
}

void CCameraManager::FocusOnTarget(const _wstring& strCameraTag, FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio)
{
	CCamera* pInstance = Get_Camera(strCameraTag);

	if (nullptr == pInstance)
		return;

	pInstance->FocusOnTarget(vTargetPos, fTargetFov, fRatio);
}

void CCameraManager::Reset_State(const _wstring& strCameraTag)
{
	CCamera* pInstance = Get_Camera(strCameraTag);

	if (nullptr == pInstance)
		return;

	pInstance->Reset_State();
}

void CCameraManager::Set_Position(const _wstring& strCameraTag, FXMVECTOR vPos)
{
	CCamera* pInstance = Get_Camera(strCameraTag);

	if (nullptr == pInstance)
		return;

	pInstance->Set_Position(vPos);
}

CCamera* CCameraManager::Get_Camera(const _wstring& strCameraTag)
{
	auto iter = m_Cameras.find(strCameraTag);

	if (m_Cameras.end() == iter)
	{
		MSG_BOX("ErrCode : No Camera");
		return nullptr;
	}
	return iter->second;
}

CCameraManager* CCameraManager::Create()
{
	return new CCameraManager;
}

void CCameraManager::Free()
{
	for (auto& pair : m_Cameras)
	{
		Safe_Release(pair.second);
	}
	m_Cameras.clear();
	CBase::Free();
}
