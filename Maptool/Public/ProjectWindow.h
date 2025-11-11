#pragma once
#include "ImgWin.h"

NS_BEGIN(Maptool)
class CModelConverter;

class CProjectWindow final : public CImgWin
{
private:
	CProjectWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CProjectWindow() = default;

public:
	virtual HRESULT	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Update_Priority(_float fDT)override;
	virtual void Update(_float fDT)override;
	virtual void Update_Late(_float fDT)override;

private:
	void Stuck_Window();
	void Ready_Lists(const _char* ListName);
	void Ready_Tabs();
	void Show_Texture_list(uint32_t iPlaceID, _wstring ProtoTag);

private:
	RECT m_MainWindowRc = {};

	uint32_t m_iGapFromWindow = {0};

	ImVec2 m_vPos = {};
	ImVec2 m_vSize = { 480.f, 640.f };

	ImGuiWindowFlags m_flags = {};
	ImGuiTabBarFlags m_TabBarFlag = {};
 
private:
	CModelConverter* m_pModelConverterWindow = { nullptr };

	vector<const char*> m_ItemNames;


public:
	static CProjectWindow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free()override;
};

NS_END