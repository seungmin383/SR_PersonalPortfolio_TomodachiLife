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
	HRESULT Load_Sound(const _wstring& SoundTag, const _string& SoundFilePath, _bool isBGM = FALSE, _bool isLoop = FALSE);

	HRESULT Begin_BGM(const _wstring& SoundTag, _float fVolume = 1.f);
	HRESULT Begin_Sound(const _wstring& SoundTag, SOUND_CHANNEL eChannel, _float fVolume = 1.f);

	HRESULT Stop_Sound(SOUND_CHANNEL eChannel);
	HRESULT Stop_AllSound();

private:
	FMOD_SOUND* Find_Sound(const _wstring SoundTag);

private:
	FMOD_SYSTEM* m_pSystem = { nullptr };
	FMOD_CHANNELGROUP* m_pMasterGroup = { nullptr };

	map<_wstring, FMOD_SOUND*> m_Sounds;

	FMOD_CHANNEL* m_pBGMChannel = { nullptr };
	map<SOUND_CHANNEL, vector<FMOD_CHANNEL*>> m_Channels;

	vector<uint32_t> m_FinChannelIndices;

public:
	static CSoundManager* Create();
	virtual void Free()override;
};

NS_END