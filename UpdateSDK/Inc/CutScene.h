#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CTimeLine_Trans;

class CCutScene final : public CBase
{
protected:
	CCutScene();
	virtual ~CCutScene() = default;

public:
	virtual HRESULT Play_CutScene(_float fDT);
	virtual HRESULT Add_TimeLine_Trans(TIMELINE_TYPE eType, CTimeLine_Trans* pTimeLine);

protected:
	_float m_fDuration = {};
	_float m_fCurTrackPosition = {};
	_bool m_isPlaying = { FALSE };

	CTimeLine_Trans* m_pCameraTimeLine = { nullptr };
	vector<CTimeLine_Trans*> m_pActorTimeLine;

public: 
	virtual void Free()override;
};

NS_END

