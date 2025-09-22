#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CLoader : public CBase
{
private:
	//디바이스랑 컨텍스트 받고
	CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLoader() = default;

public:
	//로딩할 때 실제 내가 이동하고 싶은 스테이지 아이디 받기
	HRESULT Initialize(PLACE_ID _eLoadingLevelID);
	HRESULT Loading();

	_bool isFinished() const {
		return m_isFinished;
	}

	void Output() {
		SetWindowText(g_hWnd, m_szFPS);
	}

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	PLACE_ID				m_eLoadingLevelID = { PLACE_ID::END };

private:
	HANDLE				m_hThread = {};
	CRITICAL_SECTION	m_CriticalSection = {};

	_bool				m_isFinished = { false };
	_tchar				m_szFPS[MAX_PATH] = {};

private:
#pragma region LOADING_FUCTION

	HRESULT Loading_For_Setting();
	HRESULT Loading_For_Save();
	HRESULT Loading_For_Load();
	HRESULT Loading_For_MiiEditor();
	HRESULT Loading_For_Store_Clothes();

	HRESULT Loading_For_Store_Antique();
	HRESULT Loading_For_Store_Hat();
	HRESULT Loading_For_Store_Food();
	HRESULT Loading_For_Store_Luxury();
	HRESULT Loading_For_Store_House();

	HRESULT Loading_For_Envi_Fountain();
	HRESULT Loading_For_Envi_Ocean();
	HRESULT Loading_For_Envi_Park();
	HRESULT Loading_For_Envi_Playground();
	HRESULT Loading_For_Envi_Cafe();

	HRESULT Loading_For_Etc_Community();
	HRESULT Loading_For_Etc_Music();
	HRESULT Loading_For_Etc_Ranking();
	HRESULT Loading_For_Etc_News();
	HRESULT Loading_For_Etc_Photo();

	HRESULT Loading_For_Connect_Tent();
	HRESULT Loading_For_Connect_Port();

	HRESULT Loading_For_Live_Apartment();
	HRESULT Loading_For_Live_Wedding_House();

#pragma endregion

public:
	static CLoader* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, PLACE_ID _eLoadingLevelID);
	virtual void Free() override;
};

NS_END
