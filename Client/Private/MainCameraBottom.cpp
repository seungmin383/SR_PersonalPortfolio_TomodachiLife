#include "MainCameraBottom.h"
#include "GameInstance.h"

CMainCameraBottom::CMainCameraBottom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CMainCameraBottom::CMainCameraBottom(const CMainCameraBottom& rhs)
	:CCamera(rhs)
{
}

HRESULT CMainCameraBottom::Initialize_Origin()
{
	return S_OK;
}

HRESULT CMainCameraBottom::Initialize_Clone(void* pArg)
{

	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

}

int32_t CMainCameraBottom::Update_Priority(_float fDT)
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

int32_t CMainCameraBottom::Update(_float fDT)
{
	return 0;
}

void CMainCameraBottom::Update_Late(_float fDT)
{
}

CMainCameraBottom* CMainCameraBottom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMainCameraBottom* tmp = new CMainCameraBottom(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CMainCameraBottom::Clone(void* pArg)
{
	CMainCameraBottom* tmp = new CMainCameraBottom(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CMainCameraBottom::Free()
{
	CCamera::Free();
}
