#pragma once
#include "Base.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)
class CSEAnimation;

class CAnimationManager final : public CBase
{
private:
	CAnimationManager();
	virtual ~CAnimationManager() = default;

public:
	HRESULT Initialize();

public:
	CSEAnimation* Find_Animation(const _wstring& strAnimationTag);
	vector<_wstring>* Find_AnimationMacro(const _wstring& strMacroTag);

private:
	map<_wstring, CSEAnimation*> m_Animations;

	map<_wstring, vector<_wstring>> m_AnimationMacros;

public:
	static CAnimationManager* Create();
	virtual void Free()override;
};

NS_END