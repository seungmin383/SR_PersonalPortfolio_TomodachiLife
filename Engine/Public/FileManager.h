#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CFileManager final : public CBase
{
private:
	CFileManager();
	virtual ~CFileManager() = default;

public:
	HRESULT Initialize(const _char* ConfigFilePath);
	
	HRESULT Read_JsonFile(
		const _char* szJsonFilePath,
		Json::Value* pOut);

	
private:
	nlohmann::json m_InitData = {};

	vector<nlohmann::json*>m_vecData;

public:
	static CFileManager* Create(const _char* ConfigFilePath);
	virtual void Free()override;
};

NS_END