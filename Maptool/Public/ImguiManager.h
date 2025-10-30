#pragma once
#include "Game_Defines.h"
#include "Maptool_Defines.h"
#include "Base.h"

NS_BEGIN(Maptool)
class CImgWin;

class CImguiManager final : public CBase 
{
	DECLARE_SINGLETON(CImguiManager)
private:
	CImguiManager();
	virtual ~CImguiManager() = default;

public:
	HRESULT Initialize_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Update_Imgui_Priority(_float fDT);
	void Update_Imgui(_float fDT);
	void Update_Imgui_Late(_float fDT);

	void Render_Imgui();

private:
	HRESULT Ready_Imguies();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<CImgWin*> m_ImguiWindows;

public:
	virtual void Free()override;
};

NS_END
