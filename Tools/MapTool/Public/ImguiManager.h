#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Maptool)

class CImguiManager : public CBase 
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
public:
	virtual void Free()override;
};

NS_END
