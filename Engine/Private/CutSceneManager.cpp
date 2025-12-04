#include "CutSceneManager.h"
#include "CutScene.h"

CCutSceneManager::CCutSceneManager()
{
}

HRESULT CCutSceneManager::Play_CutScene(const _string& strCutSceneTag, _float fDT)
{
	CCutScene* pCutScene = Find_CutScene(strCutSceneTag);
	if (nullptr == pCutScene)
		return E_FAIL;

	if (FAILED(pCutScene->Play_CutScene(fDT)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCutSceneManager::Add_CutScene(const _string& strCutSceneTag, CCutScene* pCutScene)
{
	CCutScene* pCut = Find_CutScene(strCutSceneTag);
	if (nullptr != pCut)
	{
		MSG_BOX("ErrCode : Already Has CutScene");
		return E_FAIL;
	}

	if (FAILED(Add_CutScene(strCutSceneTag, pCutScene)))
		return E_FAIL;

	return S_OK;
}

CCutScene* CCutSceneManager::Find_CutScene(const _string& strCutSceneTag)
{
	auto iter = m_CutScenes.find(strCutSceneTag);
	if (m_CutScenes.end() == iter)
	{
		MSG_BOX("ErrCode : No CutScene");
		return nullptr;
	}
	return iter->second;
}

CCutSceneManager* CCutSceneManager::Create()
{
	return new CCutSceneManager();
}

void CCutSceneManager::Free()
{
	for (auto& pair : m_CutScenes)
	{
		Safe_Release(pair.second);
	}
	m_CutScenes.clear();

	CBase::Free();
}
