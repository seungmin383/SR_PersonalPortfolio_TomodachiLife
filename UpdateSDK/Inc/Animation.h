#pragma once
#include "Base.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)
class CChannel;
class CBone;
class CModel;

/* 뼈대들의 시간에 따른 상태값(행렬) 을 보관함 */
/* 이 객체 하나는 특정 동작(공격모션 등)을 위한, 시간에 따른 상태값*/
/* 공격 시작부터 끝까지 하나의 애니메이션으로 */
class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(
		SE_ANIMATION& seAnim, 
		const aiAnimation* pAIAnimation,
		CModel* pModel);
	
	/* return으로 애니메이션이 끝났는지 확인 */
	_bool Update_TransformationMatrices(
		const vector<CBone*>& Bones,
		_float fDT,
		_bool isLoop);

/* 시간의 따른 상태값을 보관한다 -> 채널 하나가 담당*/
private:
	/* 특정 뼈의 시간에 따른 상태값을 보관한다. */
	vector<CChannel*> m_Channels;
	uint32_t m_iNumChannels = {};

	/* 애니메이션의 이름 보관 */
	_char m_szName[MAX_PATH] = {};

	/* 1초에 얼마나 재생해야돼 */
	/* 이걸 누적하면서 얼마나 재생되었는지를 확인하면 
	절대 안됨 -> 얘랑  fDT랑 곱해서 누적해야
	실제 애니메이션이 실행되고 있는 위치를 알 수 있다 */
	_float m_fTickPerSecond = {};

	/* 현재 애니메이션의 전체 재생 길이 */
	_float m_fDuration = {};

	/* 현재 재생 위치 */
	/* 현재 재생 시간이라면 fDT만 누적해야됨
	m_fTickPerSecond * fDT 를 누적해줄꺼임 */
	_float m_fCurTrackPosition = {};

public:
	static CAnimation* Create(
		SE_ANIMATION& seAnim,
		const aiAnimation* pAIAnimation,
		CModel* pModel);
	virtual void Free() override;
};

NS_END

/* 애니메이션의 길이 0 - - - - - - - - - 10 */
/* 10까지 가면 애니메이션이 끝났다고 정함 */
