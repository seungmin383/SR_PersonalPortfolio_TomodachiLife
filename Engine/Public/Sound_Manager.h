#pragma once
#include "Base.h"


NS_BEGIN(Engine)

class CSoundManager final : public CBase
{
private:
	CSoundManager();
	virtual ~CSoundManager() = default;

public:
	HRESULT Initialize();

	void Update();

public:
	HRESULT Load_Sound(const _wstring& SoundTag, const _wstring& SoundFilePath, _bool isBGM = FALSE, _bool isLoop = FALSE);

private:
	FMOD_SYSTEM* m_pSystem = { nullptr };
	FMOD_CHANNELGROUP* m_pMasterGroup = { nullptr };

	map<_wstring, FMOD_SOUND*> m_Sounds;
	map<_wstring, FMOD_CHANNEL*> m_Channels;

public:
	static CSoundManager* Create();
	virtual void Free()override;
};

NS_END