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
	_bool m_isPlayMacro =	{ FALSE };

	const _bool* m_pChangeState = { nullptr };

public:
	_bool Get_isEatingFood() const { return m_isEatingFood; }
	_bool Get_isFin_EatingFood() const { return m_isFin_EatingFood; }

	void Set_isEatingFood(_bool is) { m_isEatingFood = is; }
	void Set_isFin_EatingFood(_bool is) { m_isFin_EatingFood = is; }

private:
	_wstring m_strCurAnimTag = {};
	_wstring m_strPreAnimTag = {};

	_bool m_isEatingFood = { FALSE };
	_bool m_isFin_EatingFood = { FALSE };

public:
	static CSEMiiBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END