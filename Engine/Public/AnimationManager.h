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

private:
	map<_wstring, CSEAnimation*> m_Animations;

public:
	static CAnimationManager* Create();
	virtual void Free()override;
};

NS_END