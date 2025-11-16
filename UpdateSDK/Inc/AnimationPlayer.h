#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CSEAnimation;

class CAnimationPlayer final : public CComponent
{
private:
	CAnimationPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnimationPlayer(const CAnimationPlayer& rhs);
	virtual ~CAnimationPlayer() = default;

public: 
	HRESULT Initialize(const _string& strInitAnimTag);

public:
	void Play_Animation();
	void Change_Animation(const _string& strAnimaTag);

private:
	void Update_TransformationMatrix();

private:
	CSEAnimation* m_pCurAnimation = { nullptr };
	CSEAnimation* m_pPreAnimation = { nullptr };

public:
	static CAnimationPlayer* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		const _string& strInitAnimationTag);
	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END