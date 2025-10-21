#include "MainCameraTop.h"
#include "GameInstance.h"

CMainCameraTop::CMainCameraTop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CMainCameraTop::CMainCameraTop(const CMainCameraTop& rhs)
	:CCamera(rhs)
{
}

HRESULT CMainCameraTop::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMainCameraTop::Initialize_Clone(void* pArg)
{

	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CMainCameraTop::Update_Priority(_float fDT)
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

int32_t CMainCameraTop::Update(_float fDT)
{
	return 0;
}

void CMainCameraTop::Update_Late(_float fDT)
{
}

CMainCameraTop* CMainCameraTop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMainCameraTop* tmp = new CMainCameraTop(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CMainCameraTop::Clone(void* pArg)
{
	CMainCameraTop* tmp = new CMainCameraTop(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CMainCameraTop::Free()
{
	CCamera::Free();
}
