#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CSEAnimation;
class CSEBone;

class ENGINE_DLL CAnimationPlayer final : public CComponent
{
public:
	typedef struct tagAnimationPlayerDesc : public CComponent::COMPONENT_DESC
	{
		vector<CSEBone*>* m_pBones;
		_wstring strInitAnimTag;

		XMFLOAT4X4 PreTransformMatrix;
	}ANIMATIONPLAYER_DESC;

private:
	CAnimationPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnimationPlayer(const CAnimationPlayer& rhs);
	virtual ~CAnimationPlayer() = default;

public: 
	HRESULT Initialize();
	HRESULT Initialize_Clone(void* pArg);

	int32_t Update(_float fDT)override;

public:
	HRESULT Begin_Animation(const _wstring& strAnimTag, _bool isLoop);
	HRESULT Begin_Macro(const _wstring& strMacroTag);

	void Play_Animation(_float fDT);
	void Play_Macro(_float fDT);

	void Change_Animation(const _wstring& strAnimaTag, 
		_float fBlendDuration, _bool isLoop);

private:
	void Update_TransformationMatrix(_float fDT);
	void Blend_Animation(_float fDT);

public:
public:
	_bool Get_isAnimFin() const { return m_isAnimFin; }
	void  Set_isAnimFin(_bool is) { m_isAnimFin = is; }

	_bool Get_isStartAnim() const { return m_isStartAnim; }
	void  Set_isStartAnim(_bool is) { m_isStartAnim = is; }

	_bool Get_isPlayingAnim() const { return m_isPlayingAnim; }
	void  Set_isPlayingAnim(_bool is) { m_isPlayingAnim = is; }

	_bool Get_isEndAnim() const { return m_isEndAnim; }
	void  Set_isEndAnim(_bool is) { m_isEndAnim = is; }

	_bool Get_isStartMacro() const { return m_isStartMacro; }
	void  Set_isStartMacro(_bool is) { m_isStartMacro = is; }

	_bool Get_isPlayingMacro() const { return m_isPlayingMacro; }
	void  Set_isPlayingMacro(_bool is) { m_isPlayingMacro = is; }

	_bool Get_isEndMacro() const { return m_isEndMacro; }
	void  Set_isEndMacro(_bool is) { m_isEndMacro = is; }

private:
	vector<CSEBone*>* m_pBones;
	XMFLOAT4X4 m_TargetPreTransformMatrix = {};

	CSEAnimation* m_pPreAnimation = { nullptr };
	CSEAnimation* m_pCurAnimation = { nullptr };

private:
	ANIMPLAYER_TYPE m_eAnimType = { ANIMPLAYER_TYPE::END };

	_wstring m_strCurAnimTag = {};
	_wstring m_strCurMacroTag = {};

	vector<_wstring>* m_pMacro = { nullptr };

	_float m_fCurTrackPosition = {};
	_float m_fPreTrackPosition = {};

	/* 현재 애니메이션을 반복할건지 */
	_bool m_isLoop = { TRUE };

	/* 애니메이션이 바뀌었는지? */
	_bool m_isChangedAnimation = { FALSE };

	/* 애니메이션 블렌딩을 하고 있는지? */
	_bool m_isBlending = { FALSE };

	_float m_fBlendDuration = { 0.f };
	_float m_fBlendTime = { 0.f };

	/* 애니메이션 매크로 */
	uint32_t m_iCurMacroIndex = {};
	uint32_t m_iMaxMacroIndex = {};
	
	_bool m_isAnimFin = { FALSE };

	_bool m_isStartAnim = { FALSE };
	_bool m_isPlayingAnim = { FALSE };
	_bool m_isEndAnim = { FALSE };

	_bool m_isStartMacro = { FALSE };
	_bool m_isPlayingMacro = { FALSE };
	_bool m_isEndMacro = { TRUE };

public:
	static CAnimationPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END