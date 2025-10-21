#include "UICamera.h"
#include "GameInstance.h"

CUICamera::CUICamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CUICamera::CUICamera(const CUICamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CUICamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CUICamera::Initialize_Clone(void* pArg)
{

	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CUICamera::Update_Priority(_float fDT)
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

int32_t CUICamera::Update(_float fDT)
{
	return 0;
}

void CUICamera::Update_Late(_float fDT)
{
}

void CUICamera::Set_Matrices()
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ));
}

void CUICamera::Update_PipeLine()
{
}

void CUICamera::Ready_Render()
{
	Set_Matrices();
	CCamera::Update_PipeLine();
}

CUICamera* CUICamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUICamera* tmp = new CUICamera(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CUICamera::Clone(void* pArg)
{
	CUICamera* tmp = new CUICamera(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CUICamera::Free()
{
	CCamera::Free();
}
