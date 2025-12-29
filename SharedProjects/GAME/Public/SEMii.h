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
		IDLE			= 0u,
		RECEIVE_FOOD	= 1u << 0,
		ATE_FOOD		= 1u << 1,	/* 음식 다 먹음 */

	};

public:
	typedef struct tagMiiDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{
		_wstring strNickName;
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

private:
	_bool m_isFin_EatingFood = { FALSE };

public:
	static CSEMii* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END