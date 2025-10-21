#include<iostream>
#include "WorldCamera.h"
#include "GameInstance.h"

CWorldCamera::CWorldCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CWorldCamera::CWorldCamera(const CWorldCamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CWorldCamera::Initialize_Origin()
{
	return S_OK;
}

HRESULT CWorldCamera::Initialize_Clone(void* pArg)
{
	if(FAILED(CCamera::Initialize_Clone(pArg)))
		return E_FAIL;

	//AllocConsole();                          // 콘솔 창 생성
	//FILE* fp;
	//freopen_s(&fp, "CONOUT$", "w", stdout);  // 표준 출력(stdout)을 콘솔로 리디렉션
	//freopen_s(&fp, "CONOUT$", "w", stderr);  // 표준 에러(stderr)도 콘솔로 리디렉션
	//freopen_s(&fp, "CONIN$", "r", stdin);    // 표준 입력(stdin)도 콘솔로 리디렉션

	return S_OK;
}

int32_t CWorldCamera::Update_Priority(_float fDT)
{

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Go_Straight(fDT);
	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Go_Backward(fDT);
	if (GetAsyncKeyState(VK_RIGHT))
		m_pTransformCom->Go_Right(fDT);
	if (GetAsyncKeyState(VK_LEFT))
		m_pTransformCom->Go_Left(fDT);
	if (GetAsyncKeyState('U'))
		m_pTransformCom->Go_Up(fDT);
	if (GetAsyncKeyState('I'))
		m_pTransformCom->Go_Down(fDT);
	return 0;
}

int32_t CWorldCamera::Update(_float fDT)
{
	return 0;
}

void CWorldCamera::Update_Late(_float fDT)
{
	Timer += fDT;

	if (Timer >= 1.f)
	{
		isTime = TRUE;
	}
}

void CWorldCamera::Ready_Render()
{
	Set_Matrices();


	XMFLOAT4X4 tmp = {};
	XMStoreFloat4x4(&tmp,XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	//XMStoreFloat4x4(&tmp,m_pTransformCom->Get_WorldMatrix());
	/*if (isTime)
	{
		std::cout << tmp._11 << "\t" << tmp._12 << "\t" << tmp._13 << "\t" << tmp._14 << endl;
		std::cout << tmp._21 << "\t" << tmp._22 << "\t" << tmp._23 << "\t" << tmp._24 << endl;
		std::cout << tmp._31 << "\t" << tmp._32 << "\t" << tmp._33 << "\t" << tmp._34 << endl;
		std::cout << tmp._41 << "\t" << tmp._42 << "\t" << tmp._43 << "\t" << tmp._44 << endl;
		cout << "===============================================" << endl;
		isTime = FALSE;
		Timer = 0.f;
	}*/
}

void CWorldCamera::Set_Matrices()
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_ProjMatrix,
		XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ));

	//std::cout << m_ProjMatrix._11 << "\t" << m_ProjMatrix._12 << "\t" << m_ProjMatrix._13 << "\t" << m_ProjMatrix._14 << endl;
	//std::cout << m_ProjMatrix._21 << "\t" << m_ProjMatrix._22 << "\t" << m_ProjMatrix._23 << "\t" << m_ProjMatrix._24 << endl;
	//std::cout << m_ProjMatrix._31 << "\t" << m_ProjMatrix._32 << "\t" << m_ProjMatrix._33 << "\t" << m_ProjMatrix._34 << endl;
	//std::cout << m_ProjMatrix._41 << "\t" << m_ProjMatrix._42 << "\t" << m_ProjMatrix._43 << "\t" << m_ProjMatrix._44 << endl;
	//	cout << "===============================================" << endl;

}

CWorldCamera* CWorldCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldCamera* tmp = new CWorldCamera(pDevice, pContext);

	if (FAILED(tmp->Initialize_Origin()))
	{
		MSG_BOX("ErrCode : Food Create Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

CObject* CWorldCamera::Clone(void* pArg)
{
	CWorldCamera* tmp = new CWorldCamera(*this);

	if (FAILED(tmp->Initialize_Clone(pArg)))
	{
		MSG_BOX("ErrCode : Food Clone Failed");
		Safe_Release(tmp);
		return nullptr;
	}

	return tmp;
}

void CWorldCamera::Free()
{
	FreeConsole();
	CCamera::Free();
}
