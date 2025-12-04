#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCutScene;

class CCutSceneManager final : public CBase
{
private:
	CCutSceneManager();
	virtual ~CCutSceneManager() = default;

public:
	HRESULT Play_CutScene(const _string& strCutSceneTag, _float fDT);
	HRESULT Add_CutScene(const _string& strCutSceneTag, CCutScene* pCutScene);

private:
	CCutScene* Find_CutScene(const _string& strCutSceneTag);

private:
	map<_string, CCutScene*> m_CutScenes;

public:
	static CCutSceneManager* Create();
	virtual void Free()override;
};

NS_END