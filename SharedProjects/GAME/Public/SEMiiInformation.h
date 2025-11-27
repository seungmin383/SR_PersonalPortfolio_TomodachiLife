#pragma once
#include "Game_Defines.h"

struct MIIHEADINFO_DESC
{
	uint32_t iIndex;
	XMFLOAT3 vPos;
	XMFLOAT3 vSize;
	XMFLOAT4 vSkinColor;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&vPos), sizeof(XMFLOAT3));
		Out.write(reinterpret_cast<const char*>(&vSize), sizeof(XMFLOAT3));
		Out.write(reinterpret_cast<const char*>(&vSkinColor), sizeof(XMFLOAT4));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
		In.read(reinterpret_cast<char*>(&vPos), sizeof(XMFLOAT3));
		In.read(reinterpret_cast<char*>(&vSize), sizeof(XMFLOAT3));
		In.read(reinterpret_cast<char*>(&vSkinColor), sizeof(XMFLOAT4));
	}
};

struct MIIHAIRINFO_DESC
{
	uint32_t iIndex;
	XMFLOAT3 vPos;
	XMFLOAT3 vSize;
	XMFLOAT4 vHairColor;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
		Out.write(reinterpret_cast<const char*>(&vPos), sizeof(XMFLOAT3));
		Out.write(reinterpret_cast<const char*>(&vSize), sizeof(XMFLOAT3));
		Out.write(reinterpret_cast<const char*>(&vHairColor), sizeof(XMFLOAT4));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
		In.read(reinterpret_cast<char*>(&vPos), sizeof(XMFLOAT3));
		In.read(reinterpret_cast<char*>(&vSize), sizeof(XMFLOAT3));
		In.read(reinterpret_cast<char*>(&vHairColor), sizeof(XMFLOAT4));
	}
};

struct MIIEYEBROWINFO_DESC
{
	uint32_t iIndex;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
	}
};

struct MIIEYESINFO_DESC
{
	uint32_t iIndex;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
	}
};

struct MIINOSEINFO_DESC
{
	uint32_t iIndex;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
	}
};

struct MIIMOUTHINFO_DESC
{
	uint32_t iIndex;

	void WriteFile(ofstream& Out)
	{
		Out.write(reinterpret_cast<const char*>(&iIndex), sizeof(uint32_t));
	}

	void ReadFile(ifstream& In)
	{
		In.read(reinterpret_cast<char*>(&iIndex), sizeof(uint32_t));
	}
};
typedef struct tagMiiInfo
{
	MIIHEADINFO_DESC	HeadInfo {};
	MIIHAIRINFO_DESC	HairInfo {};
	MIIEYEBROWINFO_DESC EyebrowInfo{};
	MIIEYESINFO_DESC	EyesInfo{};
	MIINOSEINFO_DESC	NoseInfo{};
	MIIMOUTHINFO_DESC	MouthInfo{};

	void WriteFile(ofstream& Out)
	{
		HeadInfo.WriteFile(Out);
		HairInfo.WriteFile(Out);
		EyebrowInfo.WriteFile(Out);
		EyesInfo.WriteFile(Out);
		MouthInfo.WriteFile(Out);
	}

	void ReadFile(ifstream& In)
	{
		HeadInfo.ReadFile(In);
		HairInfo.ReadFile(In);
		EyebrowInfo.ReadFile(In);
		EyesInfo.ReadFile(In);
		MouthInfo.ReadFile(In);
	}

}MIIINFO_DESC;

