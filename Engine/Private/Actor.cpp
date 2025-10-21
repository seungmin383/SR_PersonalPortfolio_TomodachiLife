#include "Actor.h"
#include "GameInstance.h"

CActor::CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CObject(pDevice, pContext)
{
}

CActor::CActor(const CActor& rhs)
	:CObject(rhs)
{
}

HRESULT CActor::Initialize_Origin()
{
	return S_OK;
}

HRESULT CActor::Initialize_Clone(void* pArg)
{
	if (FAILED(CObject::Initialize_Clone(pArg)))
		return E_FAIL;


	m_pTransformCom = CTransformCom::Create(m_pDevice, m_pContext);
	m_pTransformCom->Initialize_Clone(pArg);
	ACTOR_DESC* pActorDesc = static_cast<ACTOR_DESC*>(pArg);

	m_fX = pActorDesc->fX;
	m_fY = pActorDesc->fY;
	m_fZ = pActorDesc->fZ;

	m_fCX = pActorDesc->fCX;
	m_fCY = pActorDesc->fCY;
	m_fCZ = pActorDesc->fCZ;
	 
	m_pTransformCom->Set_Scale(m_fCX, m_fCY, m_fCZ);

	m_pTransformCom->Set_State(TRANSFORM_STATE::POS,
		XMVectorSet(
			m_fX,
			m_fY,
			m_fZ,
			1.f));

	m_Components.emplace(TEXT("Com_Transform"), m_pTransformCom);
	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

int32_t CActor::Update_Priority(_float fDT)
{
	return 0;
}

int32_t CActor::Update(_float fDT)
{
	return 0;
}
 
void CActor::Update_Late(_float fDT)
{
	return;
}

HRESULT CActor::Render()
{
	return S_OK;
}

HRESULT CActor::Add_Component(
	uint32_t PlaceID,
	const _wstring PrototypeTag,
	const _wstring ComponentTag,
	CComponent** ppOut,
	void* pArg)
{
	if (m_Components.find(ComponentTag) != m_Components.end())
		return E_FAIL;

	CComponent* pComponent =
		static_cast<CComponent*>(m_pGameInstance->Proto_Get_Object_Clone(
			PlaceID,
			PrototypeTag,
			pArg));

	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(ComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	return S_OK;
}

void CActor::Free()
{
	for (auto& pair : m_Components)
		Safe_Release(pair.second);
	m_Components.clear();

	Safe_Release(m_pTransformCom);
	CObject::Free();
}
