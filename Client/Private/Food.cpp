#include "Food.h"
#include "GameInstance.h"

CFood::CFood(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CActor(_pDevice, _pContext)
{
}

CFood::CFood(const CFood& rhs)
	:CActor(rhs)
{
}

HRESULT CFood::Initialize_Origin()
{
	return S_OK;
}

HRESULT CFood::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

int32_t CFood::Update_Priority(const _float _fDT)
{
	int a = 10;

	return 0;
}

int32_t CFood::Update(const _float _fDT)
{
	int a = 10;

	if (FAILED(m_pGameInstance->RenderManager_Add_RenderGroup(RENDER_GROUP::NONBLEND, this)))
	{
		MSG_BOX("ErrCode : Food Render Failed");
		return 1;
	}
	return 0;
}

void CFood::Update_Late(const _float _fDT)
{
	int a = 10;

}

HRESULT CFood::Render()
{
	int a = 0;
	return S_OK;
}

CFood* CFood::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFood* tmp = new CFood(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CFood::Clone(void* pArg)
{
	CFood* pFood = new CFood(*this);

	if (FAILED(pFood->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(pFood);
		return nullptr;
	}

	return pFood;
}

void CFood::Free()
{
	CActor::Free();
}
