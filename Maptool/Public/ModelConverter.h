#pragma once
#include "ImgComponent.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

NS_BEGIN(Maptool)

class CModelConverter :public CImgComponent
{
protected:
	CModelConverter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CModelConverter() = default;

public:
	virtual HRESULT	Initialize(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext)override;

public:
	void OpenBtn_FileExplorer();
	void TextBox_CurFilePath();

	/* 경로에서 확장자를 추출해 */
	_string Export_FileEXT(const char* pFilePath);
	
	/* 파일 탐색기를 열어 */
	_string  Open_FileExplorer();

	/* aiScene을 만들어 */
	HRESULT Create_AIScene(const char* pFilePath);

	/* wstring을 string으로 바꿔 */
	_string WString_To_String(const _wstring& wstr);

private:
	Assimp::Importer m_ToolImporter = {};
	const aiScene* m_pToolAIScene = { nullptr };

	_string m_strCurFilePath = {};
	_string m_strCurFileName = {};

public:
	static CModelConverter* Create(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	virtual void Free()override;
};

NS_END