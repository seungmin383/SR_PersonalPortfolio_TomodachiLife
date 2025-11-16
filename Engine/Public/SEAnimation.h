#pragma once
#include "Base.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)
class CSEChannel;

class CSEAnimation final : public CBase
{
private:
	CSEAnimation();
	virtual ~CSEAnimation() = default;

public:
	HRESULT Initialize(SE_ANIMATION& seAnim);

private:
	vector<CSEChannel*> m_Channels;
	
	_string m_strName = {};
	_float m_fTickPerSec = {};
	_float m_fDuration = {};

public:
	static CSEAnimation* Create(SE_ANIMATION& seAnim);
	virtual void Free()override;
};

NS_END