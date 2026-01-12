#pragma once
#include "Game_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CSEModel;
class CShaderCom;
NS_END

NS_BEGIN(Game)

class CSEMiiHead final : public CPartObject
{
public:
	typedef struct tagSEMiiHead : public CPartObject::PARTOBJECT_DESC
	{
		const int32_t* pParentState;
		vector<const XMFLOAT4X4* >* pSocketMatrix;

		MIIHEADINFO_DESC* pHeadInfo = { nullptr };
		const int32_t* pClothState;
	}SEMIIHEAD_DESC;

private:
	CSEMiiHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSEMiiHead(const CSEMiiHead& rhs);
	virtual ~CSEMiiHead() = default;

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

	uint32_t m_iCurModelIndex = {};

	const int32_t* m_pParentClothState = { nullptr };
private:
	const int32_t* m_pParentState = { nullptr };
	vector<const XMFLOAT4X4*>* m_pSocketMatrix = { nullptr };
	XMFLOAT4 m_vSkinColor = {};

	MIIHEADINFO_DESC* m_pHeadinfo = { nullptr };

public:
	static CSEMiiHead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject* Clone(void* pArg)override;
	virtual void Free()override;
};

NS_END