#pragma once
#include "Base.h"
#include"SEAnimation_Data.h"

NS_BEGIN(Engine)

class CSEChannel final : public CBase
{
private:
	CSEChannel();
	CSEChannel(const CSEChannel& rhs);
	virtual ~CSEChannel() = default;

public:
	HRESULT Initialize(SE_CHANNEL& seChannel);
	void Update_TransformationMatrix(
		_float fCurTrackPosition,
		CHANNEL_POS& ChannelPos);

    const uint32_t Get_CurKeyframeIndex()const  { return m_iCurKeyframeIndex; }

private:
	uint32_t m_iBoneIndex = {};
	vector<KEYFRAME> m_Keyframes;

	uint32_t m_iCurKeyframeIndex = {};

public:
	static CSEChannel* Create(SE_CHANNEL& seChannel);
	CSEChannel* Clone();
	virtual void Free()override;

};

NS_END