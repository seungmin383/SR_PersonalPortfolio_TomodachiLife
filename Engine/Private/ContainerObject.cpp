#include "ContainerObject.h"
#include "GameInstance.h"
#include "PartObject.h"

CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CActor(pDevice, pContext)
{
}

HRESULT CContainerObject::Initialize_Origin()
{
	return S_OK;
}

HRESULT CContainerObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

int32_t CContainerObject::Update_Priority(_float fDT)
{
	CActor::Update_Priority(fDT);

	for (auto& pair : m_PartObjects)
		pair.second->Update_Priority(fDT);

	return UPDATE_CLEAR;
}

int32_t CContainerObject::Update(_float fDT)
{
	CActor::Update(fDT);
	for (auto& pair : m_PartObjects)
		pair.second->Update(fDT);
	return UPDATE_CLEAR;
}

void CContainerObject::Update_Late(_float fDT)
{
	CActor::Update_Late(fDT);
	for (auto& pair : m_PartObjects)
		pair.second->Update_Late(fDT);
}

HRESULT CContainerObject::Render()
{
	return S_OK;
}

HRESULT CContainerObject::Add_PartObject(
	uint32_t iPlaceID, 
	const _wstring& strProtoTag, 
	const _wstring& strPartObjectTag, 
	void* pArg)
{
	if (nullptr != Find_PartObject(strPartObjectTag))
	{
		MSG_BOX("ErrCode : Already Has PartObject");
		return E_FAIL;
	}

	CPartObject* pInstance =
		dynamic_cast<CPartObject*>(
			m_pGameInstance->Proto_Get_Object_Clone(iPlaceID, strProtoTag, pArg));

	if (nullptr == pInstance)
		return E_FAIL;

	m_PartObjects.emplace(strPartObjectTag, pInstance);

	return S_OK;
}

CPartObject* CContainerObject::Find_PartObject(const _wstring& strPartObjectTag)
{
	auto iter = m_PartObjects.find(strPartObjectTag);

	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}

void CContainerObject::Free()
{
	CActor::Free();

	for (auto& pair : m_PartObjects)
		Safe_Release(pair.second);
	m_PartObjects.clear();

}
