#pragma once
#include "Base.h"
#include "SEAnimation_Data.h"

NS_BEGIN(Engine)

class CBone;
class CModel;

/* 어떤 시간에, 어떤 상태를 취한다 라는 것들을 저장하기 위함*/

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(
		SE_CHANNEL& seChannel,
		const aiNodeAnim* pAIChannel,
		CModel* pModel);
	
	/* 여기서 행렬을 만들어서 
	뼈가 가지는 TransformationMatrix를 갱신해줌 */
	void Update_TransformationMatrix(
		const vector<CBone*>& Bones,
		_float fCurTrackPosition);

private:
	uint32_t m_iNumKeyFrames = {};
	/* 상태를 표한하기 위해 구조체를 선언한 이유는? */
	vector<KEYFRAME>	m_KeyFrames;

	int32_t m_iBoneIndex = {};

	/* 현재 키프레임 인덱스 */
	uint32_t m_iCurKeyframeIndex = {};

public:
	/* aiNodeAnim -> 애니메이션을 위한 상태를 가짐 */
	static CChannel* Create(
		SE_CHANNEL& seChannel,
		const aiNodeAnim* pAIChannel,
		CModel* pModel);

	virtual void Free()override;
};

NS_END