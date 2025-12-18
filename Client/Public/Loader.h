#pragma once
#include "Game_Defines.h"
#include "Client_Defines.h"

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CLoader : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(PLACEID NextPlaceID);
	HRESULT Loading();

	_bool isFinished() const 
	{
		return m_isFinished;
	}

	void Output() 
	{
		SetWindowText(g_hWnd, m_szFPS);
	}

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	PLACEID					m_eNextPlaceID = { PLACEID::END };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	HANDLE					m_hThread = {};
	CRITICAL_SECTION		m_CriticalSection = {};

	_bool					m_isFinished = { false };
	_tchar					m_szFPS[MAX_PATH] = {};

	const _tchar* m_szShaderPath = {nullptr};

private:
#pragma region LOADING_FUCTION
	HRESULT Loading_Loading();

	HRESULT Loading_Island();
	HRESULT Loading_Apart();
	HRESULT Loading_CityHall();
	HRESULT Loading_MiiEditor();
	HRESULT Loading_Room();
	HRESULT Loading_FoodStore();
	HRESULT Loading_Developer();
	
#pragma endregion

private:

public:
	static CLoader* Create(
		ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext,
		PLACEID NextPlaceID);

	virtual void Free() override;
};

NS_END
