#include "Place_CutScene.h"

CPlace_CutScene::CPlace_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlace(pDevice, pContext)
{
}

HRESULT CPlace_CutScene::Initialize()
{
	return S_OK;
}

int32_t CPlace_CutScene::Update(_float fDT)
{
	return 0;
}

HRESULT CPlace_CutScene::Render_Priority()
{
	return S_OK;
}

HRESULT CPlace_CutScene::Render_Late()
{
	return S_OK;
}

CPlace_CutScene* CPlace_CutScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

}

void CPlace_CutScene::Free()
{
	CPlace::Free();
}
