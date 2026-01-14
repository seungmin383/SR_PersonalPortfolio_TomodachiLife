#include "Camera.h"
#include "TransformCom.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera& rhs)
	:CActor(rhs)
{
}

HRESULT CCamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void* pArg)
{
	CAMERA_DESC* tDesc = static_cast<CAMERA_DESC*>(pArg);
	m_vEye		= tDesc->vEye;
	m_vAt		= tDesc->vAt;

	m_fAspect	= tDesc->fAspect;
	m_fFov		= tDesc->fFov;

	m_fNearZ	= tDesc->fNearZ;
	m_fFarZ		= tDesc->fFarZ;

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(tDesc->fCX, tDesc->fCY, 1.f);
	m_pTransformCom->Set_State(TRANSFORM_STATE::POS, XMVectorSetW(XMLoadFloat3(&m_vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vAt), 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));

	return S_OK;
}

int32_t CCamera::Update_Priority(_float fDT)
{

	return 0;
}

int32_t CCamera::Update(_float fDT)
{
	return 0;
}

void CCamera::Update_Late(_float fDT)
{
	Set_Matrices();
}

void CCamera::Set_Matrices()
{
}


void CCamera::Update_PipeLine()
{
}

void CCamera::FocusOnTarget(FXMVECTOR vTargetPos, _float fTargetFov, _float fRatio)
{
	m_pTransformCom->LookAtLerp(vTargetPos, fRatio);

	m_fFov += ((fTargetFov - m_fFov) * fRatio);

	Set_Matrices();
}

void CCamera::Reset_State()
{
}

void CCamera::Set_Position(FXMVECTOR vPos)
{
	m_pTransformCom->Set_State(TRANSFORM_STATE::POS, vPos);
}

void CCamera::Free()
{
	CActor::Free();
}
