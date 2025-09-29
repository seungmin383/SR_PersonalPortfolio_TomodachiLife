#include "TransformCom.h"

CTransformCom::CTransformCom(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CComponent(_pDevice, _pContext)
{
}

CTransformCom::CTransformCom(const CTransformCom& rhs)
	:CComponent(rhs)
{
}

HRESULT CTransformCom::Initialize_Origin()
{
	return E_NOTIMPL;
}

HRESULT CTransformCom::Initialize_Clone(void* _pArg)
{
	return E_NOTIMPL;
}

int32_t CTransformCom::Update_Priority(const _float _fDT)
{
	return 0;
}

int32_t CTransformCom::Update(const _float _fDT)
{
	return 0;
}

void CTransformCom::Update_Late(const _float _fDT)
{
}

HRESULT CTransformCom::Render()
{
	return E_NOTIMPL;
}

void CTransformCom::Go_Straight(const _float& _fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vLook = Get_State(TRANSFORM_STATE::LOOK);

	vPos += XMVector3Normalize(vLook) * m_fSpeedPerSecond * _fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Left(const _float& _fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vRight = Get_State(TRANSFORM_STATE::RIGHT);

	vPos -= XMVector3Normalize(vRight) * m_fSpeedPerSecond * _fDT;
	Set_State(TRANSFORM_STATE::RIGHT, vPos);
}

void CTransformCom::Go_Right(const _float& _fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vRight = Get_State(TRANSFORM_STATE::RIGHT);

	vPos += XMVector3Normalize(vRight) * m_fSpeedPerSecond * _fDT;
	Set_State(TRANSFORM_STATE::RIGHT, vPos);
}

void CTransformCom::Go_Backward(const _float& _fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vLook = Get_State(TRANSFORM_STATE::LOOK);

	vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSecond * _fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

CTransformCom* CTransformCom::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTransformCom* pTransformCom = new CTransformCom(_pDevice, _pContext);
	
	if (FAILED(pTransformCom->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : TransformCom Create Failed");
		Safe_Release(pTransformCom);
		return nullptr;
	}

	return pTransformCom;
}

CObject* CTransformCom::Clone(void* _pArg)
{
	CTransformCom* pTransformCom = new CTransformCom(*this);

	if(FAILED(pTransformCom->Initialize_Clone(_pArg)))
	{
		MSG_BOX("ErrCode : TranformCom Clone Failed");
		Safe_Release(pTransformCom);
		return nullptr;
	}
	return pTransformCom;
}

void CTransformCom::Free()
{
	CComponent::Free();
}
