#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CCutScene abstract : public CBase
{
protected:
	CCutScene();
	virtual ~CCutScene() = default;

public:
	virtual void Begin_CutScene();
	virtual void Update_CutScene(_float fDT);
	virtual void End_CutScene();

protected:
	_float m_fCurTrackPosition = {};

	_bool m_isPlaying = { FALSE };
public: 
	virtual void Free()override;
};

NS_END