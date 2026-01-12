#pragma once
#include "Game_Defines.h"
#include "PartObject.h"
#include "GameDirector.h"

NS_BEGIN(Engine)

class CShaderCom;
class CTextureCom;
class CRectBufferCom;

NS_END

NS_BEGIN(Game)

class CFood_Item final : public CPartObject
{
public:
	typedef struct tagFoodItemDesc : public CPartObject::PARTOBJECT_DESC
	{
		const int32_t* pParentState;
		const int32_t* pClothState;

		vector<const XMFLOAT4X4* >* pSocketMatrix;
	}FOODITEM_DESC;

protected:
	CFood_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFood_Item(const CFood_Item& rhs);
	virtual ~CFood_Item() = default;

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
	CShaderCom* m_pShaderCom = { nullptr };
	CTextureCom* m_pTextureCom = { nullptr };
	CRectBufferCom* m_pRectBufferCom = { nullptr };

	vector<const XMFLOAT4X4*>* m_pSocketMatrix = { nullptr };

	const int32_t* m_pParentState = { nullptr };
	const int32_t* m_pParentClothState = { nullptr };
	uint32_t m_iTextureIndex = {};

public:
	static CFood_Item* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END