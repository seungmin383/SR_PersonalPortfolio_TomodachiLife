#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLayer;
class CObject;
class CGameInstance;

class CObjectManager : public CBase
{
private:
	CObjectManager();
	virtual ~CObjectManager() = default;

private:
	HRESULT Initialize(uint32_t FinalPlace);
public:
	int32_t Update_Priority(_float fDT);
	int32_t Update(_float fDT);
	void	Update_Late(_float fDT);

public:
	HRESULT Add_Object_in_Layer(
		uint32_t PlaceID,
		const _wstring LayerTag,
		CObject* pObject);

	HRESULT Clear_Objects(uint32_t PlaceID);

	HRESULT Enable_Layer (uint32_t iPlaceID, const _wstring& strLayerTag);
	HRESULT Disable_Layer(uint32_t iPlaceID, const _wstring& strLayerTag);

private:
	map<const _wstring, CLayer*>* m_Layers = { nullptr };
	vector<unordered_set<_wstring>> m_DisableLayers;

	int32_t m_iFlag = {};
	uint32_t m_iMaxIndex = {};

	CGameInstance* m_pGameInstace = { nullptr };

public:
	static CObjectManager* Create(uint32_t FinalPlace);
	virtual void Free() override;
};

NS_END