#pragma once
#include "Base.h"

#include "TransformCom.h"
#include "ShaderCom.h"
#include "RectBufferCom.h"
#include "TerrainBufferCom.h"
#include "TextureCom.h"
#include "Camera.h"
#include "Model.h"
#include "SEModel.h"

NS_BEGIN(Engine)
class CPrototype final : public CBase
{
private:
	CPrototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPrototype() = default;

public:
	HRESULT Initialize(uint32_t MaxIndex);

	HRESULT Add_Prototype(
		uint32_t PlaceID,
		const _wstring ObjectTag, 
		CObject* pPrototype);

	CObject* Get_Object_Clone(
		uint32_t PlaceID,
		const _wstring ProrotypeTag,
		void* pArg = nullptr);

	HRESULT Clear_Prototype(uint32_t PlaceID);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<const _wstring, CObject*>* m_Prototypes = { nullptr };
	uint32_t m_iMaxIndex = {};

public:
	static CPrototype* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		uint32_t FinalPlace);

	virtual void Free() override;
};

NS_END