#include "TransformCom.h"
#include "ShaderCom.h"

CTransformCom::CTransformCom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CTransformCom::CTransformCom(const CTransformCom& rhs)
	:CComponent(rhs),
	m_fSpeedPerSecond(rhs.m_fSpeedPerSecond),
	m_fRotationPerSecond(rhs.m_fRotationPerSecond),
	m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT CTransformCom::Initialize_Origin()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CTransformCom::Initialize_Clone(void* _pArg)
{
	if (nullptr == _pArg)
		return S_OK;

	OBJECT_DESC* tmp = static_cast<OBJECT_DESC*> (_pArg);

	m_fRotationPerSecond = tmp->fRotationPerSecond;
	m_fSpeedPerSecond = tmp->fSpeedPerSecond;

	return S_OK;
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
	return S_OK;
}

XMVECTOR CTransformCom::Get_State(TRANSFORM_STATE eState) const
{
	return XMLoadFloat4x4(&m_WorldMatrix).r[static_cast<uint32_t>(eState)];
}

_float3 CTransformCom::Get_Scale()
{
	return _float3{
	XMVectorGetX(XMVector3Length(Get_State(TRANSFORM_STATE::RIGHT))),
	XMVectorGetX(XMVector3Length(Get_State(TRANSFORM_STATE::UP))),
	XMVectorGetX(XMVector3Length(Get_State(TRANSFORM_STATE::LOOK))) };
}

void CTransformCom::Set_Scale(_float ScaleX, _float ScaleY, _float ScaleZ)
{
	Set_State(TRANSFORM_STATE::RIGHT, 
		XMVector3Normalize(Get_State(TRANSFORM_STATE::RIGHT)) * ScaleX);

	Set_State(TRANSFORM_STATE::UP, 
		XMVector3Normalize(Get_State(TRANSFORM_STATE::UP)) * ScaleY);

	Set_State(TRANSFORM_STATE::LOOK, 
		XMVector3Normalize(Get_State(TRANSFORM_STATE::LOOK)) * ScaleZ);
}

void CTransformCom::Set_State(TRANSFORM_STATE eState, FXMVECTOR vState)
{
	XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[static_cast<uint32_t>(eState)]), vState);
}

void CTransformCom::Set_State(TRANSFORM_STATE eState, const _float4& vState)
{
	memcpy(&m_WorldMatrix.m[static_cast<uint32_t>(eState)], &vState, sizeof(vState));
}

void CTransformCom::Go_Straight(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vLook = Get_State(TRANSFORM_STATE::LOOK);

	vPos += XMVector3Normalize(vLook) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Left(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vRight = Get_State(TRANSFORM_STATE::RIGHT);
	
	_float4 tmp;
	XMStoreFloat4(&tmp, vPos);
	_float4 src = tmp;

	vPos -= XMVector3Normalize(vRight) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Right(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vRight = Get_State(TRANSFORM_STATE::RIGHT);

	vPos += XMVector3Normalize(vRight) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Backward(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vLook = Get_State(TRANSFORM_STATE::LOOK);

	vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Up(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vUp = Get_State(TRANSFORM_STATE::UP);

	vPos += XMVector3Normalize(vUp) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Go_Down(_float fDT)
{
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR vUp = Get_State(TRANSFORM_STATE::UP);

	vPos -= XMVector3Normalize(vUp) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPos);
}

void CTransformCom::Ratate(FXMVECTOR vAxis, _float Radian)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vRight = XMVECTOR{ 1.f, 0.f, 0.f, 0.f } * fScale.x;
	XMVECTOR vUp	= XMVECTOR{ 0.f, 1.f, 0.f, 0.f } * fScale.y;
	XMVECTOR vLook	= XMVECTOR{ 0.f, 0.f, 1.f, 0.f } * fScale.z;

	XMMATRIX RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);

	// XMVector3TransformNormal	 -> w를 0으로 밀어버림
	// XMVector3TransformCrood	 -> w를 1로 밀어버림
	// 만약 내가 w 값을 신뢰할 수 있다면 
	// XMVector4Transform

	vRight	= XMVector3TransformNormal(vRight, RotationMatrix);
	vUp		= XMVector3TransformNormal(vUp, RotationMatrix);
	vLook	= XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(TRANSFORM_STATE::RIGHT, vRight);
	Set_State(TRANSFORM_STATE::UP, vUp);
	Set_State(TRANSFORM_STATE::LOOK, vLook);
}

void CTransformCom::Ratate(_float fRadianX, _float fRadianY, _float fRadianZ)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vRight = XMVECTOR{ 1.f, 0.f, 0.f, 0.f } *fScale.x;
	XMVECTOR vUp	= XMVECTOR{ 0.f, 1.f, 0.f, 0.f } *fScale.y;
	XMVECTOR vLook	= XMVECTOR{ 0.f, 0.f, 1.f, 0.f } *fScale.z;

	XMVECTOR vQuaternion = XMQuaternionRotationRollPitchYaw(fRadianX, fRadianY, fRadianZ);
	XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	vRight	= XMVector3TransformNormal(vRight, RotationMatrix);
	vUp		= XMVector3TransformNormal(vUp, RotationMatrix);
	vLook	= XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(TRANSFORM_STATE::RIGHT, vRight);
	Set_State(TRANSFORM_STATE::UP, vUp);
	Set_State(TRANSFORM_STATE::LOOK, vLook);
}

void CTransformCom::Turn(FXMVECTOR vAxis, _float fDT)
{
	XMVECTOR vRight	= Get_State(TRANSFORM_STATE::RIGHT);
	XMVECTOR vUp	= Get_State(TRANSFORM_STATE::UP);
	XMVECTOR vLook	= Get_State(TRANSFORM_STATE::LOOK);

	XMMATRIX RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSecond * fDT);

	vRight	= XMVector3TransformNormal(vRight, RotationMatrix);
	vUp		= XMVector3TransformNormal(vUp, RotationMatrix);
	vLook	= XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(TRANSFORM_STATE::RIGHT, vRight);
	Set_State(TRANSFORM_STATE::UP, vUp);
	Set_State(TRANSFORM_STATE::LOOK, vLook);
}

void CTransformCom::TurnX(_float fDT)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vRight = XMVector3Normalize(Get_State(TRANSFORM_STATE::RIGHT));
	XMVECTOR vUp	= XMVector3Normalize(Get_State(TRANSFORM_STATE::UP));
	XMVECTOR vLook	= XMVector3Normalize(Get_State(TRANSFORM_STATE::LOOK));

	XMMATRIX RotationMatrix = XMMatrixRotationAxis(vRight ,m_fRotationPerSecond * fDT);

	vRight	= XMVector3Normalize(XMVector3TransformNormal(vRight, RotationMatrix));
	vUp		= XMVector3Normalize(XMVector3TransformNormal(vUp, RotationMatrix));
	vLook	= XMVector3Normalize(XMVector3TransformNormal(vLook, RotationMatrix));

	vUp		= XMVector3Normalize(XMVector3Cross(vLook, vRight));
	vLook	= XMVector3Normalize(XMVector3Cross(vRight, vUp));

	Set_State(TRANSFORM_STATE::RIGHT,	vRight * fScale.x);
	Set_State(TRANSFORM_STATE::UP,		vUp * fScale.y);
	Set_State(TRANSFORM_STATE::LOOK,	vLook * fScale.z);
}

void CTransformCom::TurnY(_float fDT)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vRight = XMVector3Normalize((Get_State(TRANSFORM_STATE::RIGHT)));
	XMVECTOR vUp	= XMVector3Normalize((Get_State(TRANSFORM_STATE::UP)));
	XMVECTOR vLook	= XMVector3Normalize((Get_State(TRANSFORM_STATE::LOOK)));

	XMMATRIX RotationMatrix = XMMatrixRotationAxis(vUp, m_fRotationPerSecond * fDT);

	vRight	= XMVector3Normalize((XMVector3TransformNormal(vRight, RotationMatrix)));
	vUp		= XMVector3Normalize((XMVector3TransformNormal(vUp, RotationMatrix)));
	vLook	= XMVector3Normalize((XMVector3TransformNormal(vLook, RotationMatrix)));

	vLook	= XMVector3Normalize(XMVector3Cross(vRight, vUp));
	vRight	= XMVector3Normalize(XMVector3Cross(vUp, vLook));

	Set_State(TRANSFORM_STATE::RIGHT,	vRight * fScale.x);
	Set_State(TRANSFORM_STATE::UP,		vUp * fScale.y);
	Set_State(TRANSFORM_STATE::LOOK,	vLook * fScale.z);
}

void CTransformCom::TurnZ(_float fDT)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vRight = XMVector3Normalize((Get_State(TRANSFORM_STATE::RIGHT)));
	XMVECTOR vUp	= XMVector3Normalize((Get_State(TRANSFORM_STATE::UP)));
	XMVECTOR vLook	= XMVector3Normalize((Get_State(TRANSFORM_STATE::LOOK)));

	XMMATRIX RotationMatrix = XMMatrixRotationAxis(vLook, m_fRotationPerSecond * fDT);

	vRight	= XMVector3Normalize((XMVector3TransformNormal(vRight, RotationMatrix)));
	vUp		= XMVector3Normalize((XMVector3TransformNormal(vUp, RotationMatrix)));
	vLook	= XMVector3Normalize((XMVector3TransformNormal(vLook, RotationMatrix)));

	vRight	= XMVector3Normalize(XMVector3Cross(vUp, vLook	));
	vUp		= XMVector3Normalize(XMVector3Cross(vLook, vRight));

	Set_State(TRANSFORM_STATE::RIGHT,	vRight * fScale.x);
	Set_State(TRANSFORM_STATE::UP,		vUp * fScale.y);
	Set_State(TRANSFORM_STATE::LOOK,	vLook * fScale.z);
}

void CTransformCom::LookAt(FXMVECTOR vPoint)
{
	_float3 fScale = Get_Scale();

	XMVECTOR vPos	= Get_State(TRANSFORM_STATE::POS);

	XMVECTOR vLook	= XMVector3Normalize(vPoint - vPos);
	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(XMVECTOR{ 0.f, 1.f, 0.f, 0.f }, vLook));
	XMVECTOR vUp	= XMVector3Normalize(XMVector3Cross(vLook, vRight));

	Set_State(TRANSFORM_STATE::RIGHT,	vRight * fScale.x);
	Set_State(TRANSFORM_STATE::UP,		vUp * fScale.y);
	Set_State(TRANSFORM_STATE::LOOK,	vLook * fScale.z);
}

void CTransformCom::LookAtLerp(FXMVECTOR vPoint, _float fRatio)
{
	_float3 fScale = Get_Scale();
	XMVECTOR vPos = Get_State(TRANSFORM_STATE::POS);

	XMVECTOR vTargetLook = XMVector3Normalize(vPoint - vPos);
	XMVECTOR vCurLook = XMVector3Normalize(Get_State(TRANSFORM_STATE::LOOK));

	XMVECTOR vLerpLook = XMVector3Normalize( XMVectorLerp(vCurLook, vTargetLook, fRatio));

	XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(XMVECTOR{ 0.f, 1.f, 0.f, 0.f }, vLerpLook));
	XMVECTOR vUp = XMVector3Normalize(XMVector3Cross(vLerpLook, vRight));

	Set_State(TRANSFORM_STATE::RIGHT, vRight * fScale.x);
	Set_State(TRANSFORM_STATE::UP, vUp * fScale.y);
	Set_State(TRANSFORM_STATE::LOOK, vLerpLook * fScale.z);
}

void CTransformCom::Chase(FXMVECTOR vTarget, _float fDT, _float fLeastDist)
{
	XMVECTOR     vPosition = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR     vMoveDir = vTarget - vPosition;

	if (fLeastDist <= XMVectorGetX(XMVector3Length(vMoveDir)))
		vPosition += XMVector3Normalize(vMoveDir) * m_fSpeedPerSecond * fDT;

	Set_State(TRANSFORM_STATE::POS, vPosition);
}

void CTransformCom::Set_Target(const XMFLOAT4& vTarget)
{
	m_isArrive = FALSE;
	m_vTarget = vTarget;
}

void CTransformCom::Go_Auto(_float fDT)
{
	if (m_isArrive)
		return;

	XMVECTOR     vPosition = Get_State(TRANSFORM_STATE::POS);
	XMVECTOR     vMoveDir = XMLoadFloat4(&m_vTarget) - vPosition;

	if (0.1f >= XMVectorGetX(XMVector3Length(vMoveDir)))
	{
		m_isArrive = TRUE;
		return;
	}

	vPosition += XMVector3Normalize(vMoveDir) * m_fSpeedPerSecond * fDT;
	Set_State(TRANSFORM_STATE::POS, vPosition);
}

HRESULT CTransformCom::Bind_ShaderResource(CShaderCom* pShader, const _char* WorldMatrixTag)
{
	return pShader->Bind_Matrix(WorldMatrixTag, &m_WorldMatrix);
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
