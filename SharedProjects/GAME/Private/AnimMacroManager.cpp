#include "AnimMacroManager.h"
#include "GameInstance.h"

#define ANIM_MACRO_DATA_FILE_PATH L"../../SharedProjects/GAME/GameData/AnimMacroData.json" 

IMPLEMENT_SINGLETON(CAnimMacroManager)


CAnimMacroManager::CAnimMacroManager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CAnimMacroManager::Initialize()
{
	return S_OK;
}

HRESULT CAnimMacroManager::Load_AnimMacros()
{
	ifstream in(ANIM_MACRO_DATA_FILE_PATH);

	if (!in.is_open())
		return E_FAIL;

	nlohmann::json j;
	in >> j;

	m_AnimMacros.clear();

	for (auto iter = j.begin(); iter != j.end(); ++iter)
	{
		const _string key = iter.key();
		const nlohmann::json& Arr = iter.value();

		vector<_wstring> macro;
		macro.reserve(Arr.size());

		for (const auto& value : Arr)
		{
			if (!value.is_string())
				continue;

			macro.push_back(StrToWstr(value.get<_string>()));

		}

		m_AnimMacros.emplace(StrToWstr(key), move(macro));
	}

	return S_OK;
}

void CAnimMacroManager::Free()
{
	Safe_Release(m_pGameInstance);
	CBase::Free();
}
