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
	vector<_float>* Find_AnimationSpeeds(const _wstring& strMacroTag);
	vector<uint32_t>* Find_AnimationLoopTime(const _wstring& strMacroTag);

private:
	map<_wstring, CSEAnimation*> m_Animations;

	map<_wstring, vector<_wstring>> m_AnimationMacros;
	map<_wstring, vector<_float>> m_AnimationSpeeds;
	map<_wstring, vector<uint32_t>> m_AnimationLoopTime;
	
public:
	static CAnimationManager* Create();
	virtual void Free()override;
};

NS_END