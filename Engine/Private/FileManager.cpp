#include "FileManager.h"

CFileManager::CFileManager()
{
}

HRESULT CFileManager::Initialize(const _char* ConfigFilePath)
{
	// Open Config.json
	ifstream fInput(ConfigFilePath);
	
	if (!fInput.is_open())
	{
		MSG_BOX("ErrCode : Config File Load Failed");
		return E_FAIL;
	}

	fInput >> m_InitData;

	// 저장된 json 경로들 불러오기
	uint32_t iNumFiles = {};
	if (m_InitData.contains("NumFilePath"))
	{
		iNumFiles = m_InitData["NumFilePath"].get<uint32_t>();
	}

	m_InitData["tmp"] = nlohmann::json::array();
	m_InitData["tmp"].push_back({ {"ProtoTag","Player"},{"Pos",{0,1,2}},{"HP",100} });
	m_InitData["tmp"].push_back({ {"ProtoTag","Enemy"},{"Pos",{5,0,3}},{"HP",50} });


	for (uint32_t i = 0; i < iNumFiles; i++)
	{
	}

	fInput.close();

	ofstream fOutput(ConfigFilePath);
	if (!fOutput.is_open())
	{
		MSG_BOX("ErrCode : Config json Open Failed");
		return E_FAIL;
	}
	fOutput << m_InitData.dump(4);
	fOutput.close();

	return S_OK;
}

HRESULT CFileManager::Read_JsonFile(
	const _char* szJsonFilePath,
	Json::Value* pOut)
{
	ifstream fInput(szJsonFilePath);

	if (!fInput.is_open())
	{
		MSG_BOX("ErrCode : Json File Read Failed");
		return E_FAIL;
	}

	fInput >> *pOut;
	
	fInput.close();


	return S_OK;
}

CFileManager* CFileManager::Create(const _char* ConfigFilePath)
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
