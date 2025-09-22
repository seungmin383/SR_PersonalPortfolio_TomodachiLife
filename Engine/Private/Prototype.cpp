#include "Prototype.h"
#include "Object.h"

CPrototype::CPrototype()
{
}

HRESULT CPrototype::Add_Prototype(int32_t _ObjectID, CObject* _Origin)
{
	if (nullptr == _Origin)
		return E_FAIL;

	if (m_mapOrigin.find(_ObjectID) == m_mapOrigin.end())
	{
		m_mapOrigin.emplace(_ObjectID, _Origin);
		return S_OK;
	}
	else
	{
		MSG_BOX("ErrCode : Origin Aready There");
		return E_FAIL;
	}
}

CObject* CPrototype::Get_Object_Clone(int32_t _ObjectID)
{
	auto iter = m_mapOrigin.find(_ObjectID);

	if (iter == m_mapOrigin.end())
		return nullptr;

	return iter->second->Clone();
}

void CPrototype::Free()
{
	for (auto& pair : m_mapOrigin)
	{
		if (nullptr != pair.second)
			Safe_Release(pair.second);
	}
	m_mapOrigin.clear();

	CBase::Free();
}
