#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CActor;

class CTimeLine_Trans final : public CBase
{
public:
	typedef struct tagTimeLineTransDesc
	{
		CActor* pOwner;

	}TIMELINE_TRANS_DESC;

private:
	CTimeLine_Trans();
	virtual ~CTimeLine_Trans() = default;

public:
	HRESULT Initialize(TIMELINE_TRANS_DESC* pDesc);
	void Update_Timeline(_float fTrackPosition, _float fDT);

	void Add_CuTFrame(const CUTFRAME& CutFrame);

private:
	void Calc_CutFrameIndex(_float fTrackPosition);

private:
	CActor* m_pOwner = { nullptr };

	vector<CUTFRAME> m_CutFrames;

	uint32_t m_iCutFrameIndex = {};
	uint32_t m_iPreFrameIndex = {};

public:
	static CTimeLine_Trans* Create(TIMELINE_TRANS_DESC* pDesc);
	virtual void Free()override;
};

NS_END