#include "MiiManager.h"

IMPLEMENT_SINGLETON(CMiiManager)

CMiiManager::CMiiManager()
{
}

HRESULT CMiiManager::Initialize()
{

	return S_OK;
}

void CMiiManager::Begin_Edit(MIIINFO_DESC& CurMiiDesc)
{
	m_EditDesc = CurMiiDesc;
}

HRESULT CMiiManager::Save_MiiData(const _wstring& strFilePath)
{
	ofstream Out(strFilePath);
	if (!Out.is_open())
		return E_FAIL;

	nlohmann::json j = m_EditDesc.ToJson();
	Out << j.dump(4);

	MSG_BOX("Success Save : MiiData");


	return S_OK;
}

HRESULT CMiiManager::Load_MiiData(__out MIIINFO_DESC& Out, const _wstring& strFilePath)
{
	ifstream In(strFilePath);
	if (!In.is_open())
		return E_FAIL;

	nlohmann::json j;
	In >> j;
	Out.FromJson(j);
	return S_OK;
}


void CMiiManager::Free()
{
	CBase::Free();
}