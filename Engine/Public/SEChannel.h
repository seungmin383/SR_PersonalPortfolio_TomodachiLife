#pragma once
#include "Base.h"
#include"SEAnimation_Data.h"

NS_BEGIN(Engine)

class CSEChannel final : public CBase
{
private:
	CSEChannel();
	virtual ~CSEChannel() = default;

public:
	HRESULT Initialize(SE_CHANNEL& seChannel);
	void Update_TransformationMatrix(
		_float fCurTrackPosition,
		CHANNEL_POS& ChannelPos);

private:
	uint32_t m_iBoneIndex = {};
	vector<KEYFRAME> m_Keyframes;

	uint32_t m_iCurKeyframeIndex = {};

public:
	static CSEChannel* Create(SE_CHANNEL& seChannel);
	virtual void Free()override;

};

NS_END