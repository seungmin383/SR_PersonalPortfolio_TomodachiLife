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

_double CFileManager::RoundN(double v, int n) {
	const double s = std::pow(10.0, n);
	return std::round(v * s) / s;
}

void CFileManager::RoundFloatsInJson(nlohmann::json& j, int n)
{
	const double scale = std::pow(10.0, n);
	const double eps = 1e-6;

	std::function<void(nlohmann::json&)> dfs = [&](nlohmann::json& node)
		{
			if (node.is_number_float())
			{
				double v = node.get<double>();
				v = std::round(v * scale) / scale;   // 소수 n자리 반올림
				if (std::abs(v) < eps) v = 0.0;      // 아주 작은 값은 0으로 스냅
				node = v;
			}
			else if (node.is_array())
			{
				// 배열: 인덱스로 순회 (참조 유지)
				for (std::size_t i = 0; i < node.size(); ++i)
					dfs(node[i]);
			}
			else if (node.is_object())
			{
				// 객체: 키-값 순회 (C++17 구조적 바인딩 사용 안 함)
				for (nlohmann::json::iterator it = node.begin(); it != node.end(); ++it)
				{
					// it.key(); // 키가 필요하면 이렇게 사용
					dfs(it.value());   // 값에 대해 재귀
					// 구버전 nlohmann::json이라면: dfs(*it); 로 대체 가능
				}
			}
			// number_integer / string / boolean 등은 그대로 둠
		};

	dfs(j);
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
