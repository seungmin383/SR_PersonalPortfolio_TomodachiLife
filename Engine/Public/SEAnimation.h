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
	_bool Update_TransformationMatrix(
		_float fDT);

	const uint32_t Get_NumChannel() const{ return m_iNumChannel; }
	const vector<CHANNEL_POS>& Get_ChannelPos()const  { return m_ChannelPos; }

private:
	uint32_t m_iNumChannel = {};
	vector<CSEChannel*> m_Channels;
	vector<CHANNEL_POS> m_ChannelPos;

	_string m_strName = {};
	_float m_fTickPerSec = {};
	_float m_fDuration = {};
	_float m_fCurTrackPosition = {};
public:
	static CSEAnimation* Create(SE_ANIMATION& seAnim);
	virtual void Free()override;
};

NS_END