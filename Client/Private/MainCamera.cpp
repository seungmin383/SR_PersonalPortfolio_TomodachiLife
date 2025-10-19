#include "MainCamera.h"
#include "GameInstance.h"

CMainCamera::CMainCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CMainCamera::CMainCamera(const CMainCamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CMainCamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMainCamera::Initialize_Clone(void* pArg)
{

	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

}

int32_t CMainCamera::Update_Priority(_float fDT)
{

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Go_Straight(fDT);
	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Go_Backward(fDT);
	if (GetAsyncKeyState(VK_RIGHT))
		m_pTransformCom->Go_Right(fDT);
	if (GetAsyncKeyState(VK_LEFT))
		m_pTransformCom->Go_Left(fDT);

	CCamera::Update_Priority(fDT);
	return 0;
}

int32_t CMainCamera::Update(_float fDT)
{
	return 0;
}

void CMainCamera::Update_Late(_float fDT)
{
}

CMainCamera* CMainCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMainCamera* tmp = new CMainCamera(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CMainCamera::Clone(void* pArg)
{
	CMainCamera* tmp = new CMainCamera(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CMainCamera::Free()
{
	CCamera::Free();
}
