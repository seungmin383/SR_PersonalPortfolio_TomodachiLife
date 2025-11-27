#include "PartObject.h"
#include "GameInstance.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

CPartObject::CPartObject(const CPartObject& rhs)
	:CActor(rhs)
{
}


HRESULT CPartObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CPartObject::Initialize_Clone(void* pArg)
{
	PARTOBJECT_DESC* pDesc = static_cast<PARTOBJECT_DESC*>(pArg);
	m_pParentMatrix = pDesc->pParnetMatrix;

	if (FAILED(CActor::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

int32_t CPartObject::Update_Priority(_float fDT)
{
	CActor::Update_Priority(fDT);
	return 0;
}

int32_t CPartObject::Update(_float fDT)
{
	CActor::Update(fDT);
	return 0;
}

void CPartObject::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
}

HRESULT CPartObject::Render()
{
	__super::Render();
	return S_OK;
}

void CPartObject::SetUp_CombinedWorldMatrix(FXMMATRIX ParentMatrix)
{
	XMStoreFloat4x4(
		&m_CombinedWorldMatrix, 
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * ParentMatrix);
}

void CPartObject::Free()
{
	CActor::Free();


}
