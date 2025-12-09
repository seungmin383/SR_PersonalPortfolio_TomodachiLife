#include<iostream>
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
	WORLDCAMERA_DESC* pDesc = static_cast<WORLDCAMERA_DESC*>(pArg);
	m_fSensor = pDesc->fSensor;

	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CWorldCamera::Update_Priority(_float fDT)
{
	XMFLOAT4 look = {}; 
	XMStoreFloat4(&look,  m_pTransformCom->Get_State(TRANSFORM_STATE::LOOK));

	// cout << "X : " << look.x << " Y : " << look.y << " Z : " << look.z << endl;

	if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fDT);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fDT*10.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fDT);
	}

	 _long    MouseMove = {};
	 
	 if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
	 {
	 	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fSensor * MouseMove * fDT);
	 }
	 
	 if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
	 {
	 	m_pTransformCom->Turn(m_pTransformCom->Get_State(TRANSFORM_STATE::RIGHT), m_fSensor * MouseMove * fDT);
	 }

	return 0;
}

int32_t CWorldCamera::Update(_float fDT)
{
	return 0;
}

void CWorldCamera::Update_Late(_float fDT)
{
	CCamera::Update_Late(fDT);
}

void CWorldCamera::Set_Matrices()
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ));
}

void CWorldCamera::Reset_State()
{
	m_fFov = XMConvertToRadians(45.f);
	m_pTransformCom->LookAtLerp(XMVECTOR{ 0.f, 0.f, 0.f, 1.f }, 0.1f);
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
