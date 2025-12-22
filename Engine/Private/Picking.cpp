#include "Picking.h"
#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext), 
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPicking::Initialize(uint32_t iWidth, uint32_t iHeight)
{
	D3D11_TEXTURE2D_DESC Desc = {};

	Desc.Width = iWidth;
	Desc.Height = iHeight;
	Desc.MipLevels = 1;
	Desc.ArraySize = 1;
	Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	Desc.SampleDesc.Quality = 0;
	Desc.SampleDesc.Count = 1;

	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.BindFlags = 0;

	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&Desc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	m_pPickInfo = new XMFLOAT4[iWidth * iHeight];
	ZeroMemory(m_pPickInfo, sizeof(XMFLOAT4) * iWidth * iHeight);

	m_iWidth = iWidth;
	m_iHeight = iHeight;


	return S_OK;
}

void CPicking::Update()
{
	m_pGameInstance->TargetManager_Copy_Resource(TEXT("Target_Pick"), m_pTexture2D);

	D3D11_MAPPED_SUBRESOURCE SubRecource_Desc = {};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubRecource_Desc);

	memcpy(m_pPickInfo, SubRecource_Desc.pData, sizeof(XMFLOAT4) * m_iWidth * m_iHeight);

	m_pContext->Unmap(m_pTexture2D, 0);
}

_bool CPicking::Picking(XMFLOAT4* pOut)
{
	POINT MousePos = m_pGameInstance->MouseManager_Get_MousePos();

	uint32_t iIndex = MousePos.y * m_iWidth + MousePos.x;

	if (iIndex >= m_iWidth * m_iHeight || 0.f == m_pPickInfo[iIndex].w)
		return FALSE;

	*pOut = m_pPickInfo[iIndex];

	return TRUE;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, uint32_t iWidth, uint32_t iHeight)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);
	if (FAILED(pInstance->Initialize(iWidth, iHeight)))
	{
		MSG_BOX("ErrCode : CPicking Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
