#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLayer;
class CGameInstance;
class CObjectManager : public CBase
{
private:
	CObjectManager();
	virtual ~CObjectManager() = default;

private:
	HRESULT Initialize(uint32_t FinalPlace);
public:
	int32_t Update_Priority(const _float fDT);
	int32_t Update(const _float fDT);
	void	Update_Late(const _float fDT);

public:
	HRESULT Add_Object_to_Layer(
		uint32_t PlaceID,
		uint32_t LayerID, 
		const uint32_t ObjectID, 
		void* pArg);

private:
	//			 레이어 아이디 레이어 리스트
	unordered_map<uint32_t, vector<CLayer*>>* m_mapLayer = {nullptr};
	int32_t m_iFlag = {};
	uint32_t m_iMaxIndex = {};

	CGameInstance* m_pGameInstace = { nullptr };

public:
	static CObjectManager* Create(uint32_t FinalPlace);
	virtual void Free() override;
};

NS_END