#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CSEModel;
class CShaderCom;
class CCollider;

class CAnimationPlayer;

NS_END

NS_BEGIN(Game)

class CSEMiiBody final : public CPartObject
{
public:
	typedef struct tagSEMiiBodyDesc : public CPartObject::PARTOBJECT_DESC
	{
		const int32_t* pParnetState;
		const int32_t* pParentClothState;

		const _bool* pChangeState;

	}SEMIIBODY_DESC;

private:
	CSEMiiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMiiBody(const CSEMiiBody& rhs);
	virtual ~CSEMiiBody() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	void Calc_BodyState(_float fDT);

public:
	const XMFLOAT4X4* Get_SocketMatrix(const _char* pBoneName);


private:
	CSEModel* m_pSeModel = { nullptr };
	CShaderCom* m_pShader = { nullptr };
	CCollider* m_pCollider = { nullptr };
	CAnimationPlayer* m_pAnimationPlayer = { nullptr };

private:
	const int32_t* m_pParentState = { nullptr };
	const int32_t* m_pParentClothState = { nullptr };

	_bool m_isStartMacro = { FALSE };
	_bool m_isPlayingMacro =	{ FALSE };

	const _bool* m_pChangeState = { nullptr };

public:
	_bool Get_isFin_Macro() const;
	
	_bool Get_isStart_ReceiveFood_Event() const { return m_isStart_ReceiveFood_Event; }
	_bool Get_isReceivedFood() const { return m_isReceiveFood; }
	_bool Get_isEatingFood() const { return m_isEatingFood; }
	_bool Get_isReactingTaste() const { return m_isReactingTaste; }
	_bool Get_isFin_EatingFood() const { return m_isFin_EatingFood; }

public:
	_bool Get_isFin_Banter() const { return m_isFin_Banter; }

private:
	_wstring m_strCurAnimTag = {};
	_wstring m_strPreAnimTag = {};

private:
	/* 음식 받았을 때 */
	_bool m_isPlaying_ReceiveFood_Event = { FALSE };
	
	/* 친구 만나러 갔을 때 */
	_bool m_isPlaying_BeFriend_Event = { FALSE };

private:/* 밖에 알려줘야 되는 플래그 */

	/* 매크로 애니메이션이 시작 되었는지 */
	_bool m_isStart_ReceiveFood_Event = { FALSE };
	
	/* 음식을 받았는지 */
	_bool m_isReceiveFood = { FALSE };
	
	/* 음식을 먹고 있는지 */
	_bool m_isEatingFood = { FALSE };
	
	/* 음식을 다 먹었는지 */
	_bool m_isFin_EatingFood = { FALSE };

	/* 맛 평가를 하고 있는지 */
	_bool m_isReactingTaste = { FALSE };

private:
	/* 대화가 끝났는지 */
	_bool m_isFin_Banter = { FALSE };


public:
	static CSEMiiBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END