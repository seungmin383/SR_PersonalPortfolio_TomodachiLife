#include "WorldCamera.h"
#include "GameInstance.h"

CWorldCamera::CWorldCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CWorldCamera::CWorldCamera(const CWorldCamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CWorldCamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CWorldCamera::Initialize_Clone(void* pArg)
{
	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CWorldCamera::Update_Priority(_float fDT)
{

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Go_Straight(fDT);
	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Go_Backward(fDT);
	if (GetAsyncKeyState(VK_RIGHT))
		m_pTransformCom->Go_Right(fDT);
	if (GetAsyncKeyState(VK_LEFT))
		m_pTransformCom->Go_Left(fDT);
	if (GetAsyncKeyState('U'))
		m_pTransformCom->Go_Up(fDT);
	if (GetAsyncKeyState('I'))
		m_pTransformCom->Go_Down(fDT);
	return 0;
}

int32_t CWorldCamera::Update(_float fDT)
{
	return 0;
}

void CWorldCamera::Update_Late(_float fDT)
{

}

void CWorldCamera::Ready_Render()
{
	Set_Matrices();
}

void CWorldCamera::Set_Matrices()
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ));
}

CWorldCamera* CWorldCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldCamera* tmp = new CWorldCamera(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CWorldCamera::Clone(void* pArg)
{
	CWorldCamera* tmp = new CWorldCamera(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CWorldCamera::Free()
{
	CCamera::Free();
}
