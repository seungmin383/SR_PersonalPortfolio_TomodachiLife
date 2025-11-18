#pragma once
#include "Engine_Define.h"

typedef struct tagChannelData
{
	uint32_t iBoneIndex;
	vector<KEYFRAME> Keyframes;
	
	void WriteChannelFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iBoneIndex), sizeof(uint32_t));

		uint32_t NumKeyframe = static_cast<uint32_t>(Keyframes.size());
		Out.write(reinterpret_cast<const char*>(&NumKeyframe), sizeof(uint32_t));
		if (0 < NumKeyframe)
			Out.write(reinterpret_cast<const char*>(Keyframes.data()), sizeof(KEYFRAME) * NumKeyframe);
	}

	void ReadChannelFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iBoneIndex), sizeof(uint32_t));
		
		uint32_t NumKeyframe = {};
		In.read(reinterpret_cast<char*>(&NumKeyframe), sizeof(uint32_t));
		
		if (0 < NumKeyframe)
		{
			Keyframes.resize(NumKeyframe);
			In.read(reinterpret_cast<char*>(Keyframes.data()), sizeof(KEYFRAME) * NumKeyframe);
		}
	}
}SE_CHANNEL;

typedef struct tagAnimationData
{
	string strName;
	_float fTickPerSec;
	_float fDuration;
	
	vector<SE_CHANNEL>Channels;

	void WriteAnimationFile(ofstream& Out)
	{
		uint32_t NameSize = static_cast<uint32_t>(strName.size());
		Out.write(reinterpret_cast<const char*>(&NameSize), sizeof(uint32_t));
		Out.write(strName.data(), NameSize);

		Out.write(reinterpret_cast<const char*>(&fTickPerSec), sizeof(_float));
		Out.write(reinterpret_cast<const char*>(&fDuration), sizeof(_float));

		uint32_t NumChannel = static_cast<uint32_t>(Channels.size());
		Out.write(reinterpret_cast<const char*>(&NumChannel), sizeof(uint32_t));
		for (auto& ch : Channels)
			ch.WriteChannelFile(Out);

	}

	void ReadSEAnimationFile(ifstream& In)
	{
		uint32_t NameSize = {};
		In.read(reinterpret_cast<char*>(&NameSize), sizeof(uint32_t));
		strName.resize(NameSize);
		In.read(strName.data(), NameSize);

		In.read(reinterpret_cast<char*>(&fTickPerSec), sizeof(_float));
		In.read(reinterpret_cast<char*>(&fDuration), sizeof(_float));

		uint32_t NumChannel = {};
		In.read(reinterpret_cast<char*>(&NumChannel), sizeof(uint32_t));

		Channels.resize(NumChannel);
		for (uint32_t i = 0; i < NumChannel; ++i)
		{
			Channels[i].ReadChannelFile(In);
		}
	}
}SE_ANIMATION;
