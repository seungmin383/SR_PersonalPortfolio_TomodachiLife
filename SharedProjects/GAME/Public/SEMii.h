#pragma once
#include "Game_Defines.h"
#include "ContainerObject.h"
#include "GameDirector.h"

NS_BEGIN(Engine)
class CBrain;
NS_END

NS_BEGIN(Game)

class CSEMii final : public CContainerObject
{
public:
	enum STATE
	{
		IDLE			= 1u << 0,
		RECEIVE_FOOD	= 1u << 1,
		EATING_FOOD		= 1u << 2,
		ATE_FOOD		= 1u << 3,	/* 음식 다 먹음 */
		FOOD_BEST		= 1u << 4,
		FOOD_GOOD		= 1u << 5,
		FOOD_NOTBAD		= 1u << 6,
		FOOD_SOSO		= 1u << 7,
		FOOD_BAD		= 1u << 8,
		FOOD_WORST		= 1u << 9,
		FIN_FOODEVENT	= 1u << 10
	};

public:
	typedef struct tagMiiDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{
		_wstring strNickName;
		uint32_t iIndex;

	}MII_DESC;

private:
	CSEMii(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMii(const CSEMii& rhs);
	virtual ~CSEMii() = default;

public:
	virtual HRESULT Initialize_Origin()override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual int32_t Update_Priority(_float fDT)override;
	virtual int32_t Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

	virtual HRESULT Render()override;

public:
	/* 플래그들을 받아오기 */
	void Observe();

	/* 받아온 플래그를 이용해서 상태 만들기 */
	void Thinking();

	/* 상태에 따른 명령 내리기 */
	void Acting();

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

private:
	CGameDirector::GAME_DIRECTION m_eDirection = { CGameDirector::GAME_DIRECTION::END };
	CGameDirector::GAME_DIRECTION m_ePreDirection = { CGameDirector::GAME_DIRECTION::END };

private:
	int32_t m_iState = {};
	int32_t m_iClothState = {};
	_bool m_isChangeState = { FALSE };

	MIIINFO_DESC m_MiiInfo = {};

	_wstring m_strNickName = {};
	uint32_t m_iIndex = {};

private:
	_bool m_isFin_EatingFood = { FALSE };
	_bool m_isEatingFood = { FALSE };
	_bool m_isFin_Macro = { FALSE };

	_bool m_isLevelUp = { FALSE };
	_bool m_isFullnessUp = { FALSE };

public:
	static CSEMii* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END