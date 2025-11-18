#pragma once
#include "Base.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)
class CSEChannel;

class CSEAnimation final : public CBase
{
private:
	CSEAnimation();
	CSEAnimation(const CSEAnimation& rhs);
	virtual ~CSEAnimation() = default;

public:
	HRESULT Initialize(SE_ANIMATION& seAnim);
	void Update_TransformationMatrix(
		_float& fCurTrackPosition, _float fDT, _bool isChangeAnim, _bool isLoop);

	const uint32_t Get_NumChannel() const{ return m_iNumChannel; }
	const vector<CHANNEL_POS>& Get_ChannelPos()const  { return m_ChannelPos; }
	const vector<CSEChannel*>& Get_Channel()const { return m_Channels; }

private:
	uint32_t m_iNumChannel = {};
	vector<CSEChannel*> m_Channels;
	vector<CHANNEL_POS> m_ChannelPos;

	_string m_strName = {};
	_float m_fTickPerSec = {};
	_float m_fDuration = {};

public:
	static CSEAnimation* Create(SE_ANIMATION& seAnim);
	CSEAnimation* Clone();
	virtual void Free()override;
};

NS_END