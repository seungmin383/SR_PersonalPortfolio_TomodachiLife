#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CSEAnimation;
class CSEBone;

class CAnimationPlayer final : public CComponent
{
private:
	CAnimationPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnimationPlayer(const CAnimationPlayer& rhs);
	virtual ~CAnimationPlayer() = default;

public: 
	HRESULT Initialize(
		const _string& strInitAnimTag,
		vector<CSEBone*>& pBones);
	HRESULT Initialize_Clone(void* pArg);

public:
	void Play_Animation(_float fDT);
	void Change_Animation(
		const _string& strAnimaTag, 
		_float fBlendDuration,
		_bool isLoop);

private:
	void Update_TransformationMatrix(_float fDT);
	void Blend_Animation(_float fDT);

private:
	_string m_strCurAnimTag = {};
	CSEAnimation* m_pCurAnimation = { nullptr };
	CSEAnimation* m_pPreAnimation = { nullptr };

	vector<CSEBone*> m_pBones;

	/* 현재 애니메이션을 반복할건지 */
	_bool m_isLoop = { TRUE };

	/* 애니메이션이 바뀌었는지? */
	_bool m_isChangedAnimation = { FALSE };

	/* 애니메이션 블렌딩을 하고 있는지? */
	_bool m_isBlending = { TRUE };

	_float m_fBlendDuration = { 0.f };
	_float m_fBlendTime = { 0.f };

public:
	static CAnimationPlayer* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const _string& strInitAnimationTag,
		vector<CSEBone*>& pBones);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END