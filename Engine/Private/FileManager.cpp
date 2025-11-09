#include "FileManager.h"

CFileManager::CFileManager()
{
}

HRESULT CFileManager::Initialize(
	const _char* ConfigFilePath)
{
	ifstream fInput(ConfigFilePath);
	
	if (!fInput.is_open())
	{
		MSG_BOX("ErrCode : Config File Read Failed");
		return E_FAIL;
	}

	fInput >> m_InitData;
	
	m_iNumFiles = static_cast<int32_t>(m_InitData["FilePath"].size());


	for (uint32_t i = 0; i < m_iNumFiles; i++)
	{
		string strFilePath = m_InitData["FilePath"][i].get<string>();
		nlohmann::json tmp = {};
		Read_File(strFilePath, tmp);
		m_DataMap.emplace(strFilePath, tmp);
	}

	fInput.close();

	return S_OK;
}

HRESULT CFileManager::Read_File(
	const string& strFilePath,
	nlohmann::json& pOut)
{
	ifstream fInput(strFilePath);

	if (!fInput.is_open())
	{
		MSG_BOX("ErrCode : Json File Read Failed");
		return E_FAIL;
	}

	fInput >> pOut;
	fInput.close();
	return S_OK;
}

HRESULT CFileManager::Write_File(
	const string& strFilePath, 
	nlohmann::json& Data)
{
	ofstream fOutput(strFilePath);
	if (!fOutput.is_open())
	{
		MSG_BOX("ErrCode : File Read Failed");
		return E_FAIL;
	}
	fOutput << (Data).dump(1);
	fOutput.close();

	return S_OK;
}

CFileManager* CFileManager::Create(
	const _char* ConfigFilePath)
{
	CFileManager* pInstance = new CFileManager;
	
	if (FAILED(pInstance->Initialize(ConfigFilePath)))
	{
		MSG_BOX("ErrCode : FileManager Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFileManager::Free()
{
	CBase::Free();
}
