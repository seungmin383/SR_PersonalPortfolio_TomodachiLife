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
		const _string& strAnimaTag);

private:
	void Update_TransformationMatrix(_float fDT);

private:
	CSEAnimation* m_pCurAnimation = { nullptr };
	CSEAnimation* m_pPreAnimation = { nullptr };

	vector<CSEBone*> m_pBones;
	_bool m_isChangedAnimation = { FALSE };

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