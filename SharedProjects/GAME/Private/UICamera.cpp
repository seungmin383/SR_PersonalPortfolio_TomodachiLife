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
	return 0;
}

int32_t CUICamera::Update(_float fDT)
{
	return 0;
}

void CUICamera::Update_Late(_float fDT)
{
	CCamera::Update_Late(fDT);
}


void CUICamera::Set_Matrices()
{
	D3D11_VIEWPORT CurViewport = {};
	uint32_t iNumViewport = { 1 };

	HRESULT hr = m_pDevice->GetDeviceRemovedReason();
	if (FAILED(hr))
	{
		MessageBoxA(0, "DEVICE REMOVED BEFORE CLEAR!", 0, 0);
	}

	m_pContext->RSGetViewports(&iNumViewport, &CurViewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixOrthographicLH(CurViewport.Width, CurViewport.Height, m_fNearZ, m_fFarZ));
}




CUICamera* CUICamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUICamera* tmp = new CUICamera(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : UICamera Create Failed");
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
		MSG_BOX("ErrCode : UICamera Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CUICamera::Free()
{
	CCamera::Free();
}
