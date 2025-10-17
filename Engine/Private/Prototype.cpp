#include "Prototype.h"

CPrototype::CPrototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPrototype::Initialize(uint32_t MaxIndex)
{
	m_iMaxIndex = MaxIndex;
	m_Prototypes = new map<const _wstring, CObject*>[m_iMaxIndex];
	return S_OK;
}

HRESULT CPrototype::Add_Prototype(
	uint32_t PlaceID, 
	const _wstring ObjectTag, 
	CObject* pPrototype)
{
	if(PlaceID >= m_iMaxIndex)
		return E_FAIL;

	// 원본이 이미 있다
	if (m_Prototypes[PlaceID].find(ObjectTag) != m_Prototypes[PlaceID].end())
	{
		MSG_BOX("ErrCode : Prototype Already Created");
		return E_FAIL;
	}

	m_Prototypes[PlaceID].emplace(ObjectTag, pPrototype);
	return S_OK;
}

CObject* CPrototype::Get_Object_Clone(
	uint32_t PlaceID,
	const _wstring ProrotypeTag, 
	void* pArg)
{
	auto iter = m_Prototypes[PlaceID].find(ProrotypeTag);

	if ( iter == m_Prototypes[PlaceID].end())
	{
		MSG_BOX("ErrCode : Prototype isn't Created");
		return nullptr;
	}

	return iter->second->Clone(pArg);
}

HRESULT CPrototype::Clear_Prototype(uint32_t PlaceID)
{
	if (PlaceID >= m_iMaxIndex)
		return E_FAIL;

	for (auto& pair : m_Prototypes[PlaceID])
		Safe_Release(pair.second);
	m_Prototypes[PlaceID].clear();

	return S_OK;
}

CPrototype* CPrototype::Create(
	ID3D11Device* pDevice, 
	ID3D11DeviceContext* pContext,
	uint32_t FinalPlace)
{
	CPrototype* pPrototype = new CPrototype(pDevice, pContext);
	
	if (FAILED(pPrototype->Initialize(FinalPlace)))
	{
		MSG_BOX("ErrCode : Prototype Create Failed");
		Safe_Release(pPrototype);
		return nullptr;
	}

	return pPrototype;
}

void CPrototype::Free()
{
	for (uint32_t i = 0; i < m_iMaxIndex; i++)
	{
		for (auto& pair : m_Prototypes[i])
			Safe_Release(pair.second);
		m_Prototypes[i].clear();
	}

	Safe_Delete_Array(m_Prototypes);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	CBase::Free();
}
