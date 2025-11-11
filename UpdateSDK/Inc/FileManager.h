#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CFileManager final : public CBase
{
private:
	CFileManager();
	virtual ~CFileManager() = default;

public:
	HRESULT Initialize(
		const _char* ConfigFilePath);

	HRESULT Read_File(
		const string& strFilePath,
		nlohmann::json& pOut);

	HRESULT Write_File(
		const string& strFilePath,
		nlohmann::json& Data);

	_double RoundN(double v, int n);

	void RoundFloatsInJson(nlohmann::json& j, int32_t n = 6);
	
public:
	nlohmann::json& Get_Data(const string& strFilePath)
	{
		auto iter = m_DataMap.find(strFilePath);
		if (iter == m_DataMap.end())
		{
			MSG_BOX("ErrCode : No Json Data");
		}
		return iter->second;
	}

private:
	nlohmann::json m_InitData = {};
	uint32_t m_iNumFiles = {};
	map<string, nlohmann::json> m_DataMap;

public:
	static CFileManager* Create(
		const _char* ConfigFilePath);

	virtual void Free()override;
};

NS_END