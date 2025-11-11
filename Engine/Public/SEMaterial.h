#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSEMaterial final : public CBase
{
private:
	CSEMaterial();
	virtual ~CSEMaterial() = default;

public:
	void ReadSEMaterialFile(ifstream& In);

private:
	int32_t m_iIndex = {};
	vector<string> m_TexturePaths[27];

public:
	static CSEMaterial* Create();
	virtual void Free()override;
};

NS_END