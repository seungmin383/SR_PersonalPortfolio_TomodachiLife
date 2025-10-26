#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CLight final: public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	HRESULT Initialize(const LIGHT_DESC& eLight);

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_eLightDesc;
	}

private:
	LIGHT_DESC m_eLightDesc = {};

public:
	static CLight* Create(const LIGHT_DESC& eLight);
	virtual void Free()override;
};

NS_END


// diffuse - 흰색
// ambient > 재질
//
// texcoord 배수
//
// 민맵 저장은 dds 로 저장
// 
// 마스크점 
// 
// 
// 스플레팅 쉐이더
// 마스크 텍스쳐 하나로 4개까지 r값만 이용해서 
//