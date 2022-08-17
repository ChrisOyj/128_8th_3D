#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CDefault_UI;

class CWindow_UI final
	: public CImGui_Window
{
public:
	struct UI_ITEM
	{
		char szBuf[MIN_STR];
		CDefault_UI* pUI = nullptr;
		_bool bSelected = false;

	};

	struct UI_FILE
	{
		string strFileName;
		_bool bSelected = false;
	};

private:
	CWindow_UI();
	virtual ~CWindow_UI();

public:
	static CWindow_UI* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	vector<UI_FILE>		m_vecUIFile;
	vector<UI_ITEM>		m_vecUI;
	_uint				m_iCurrentIdx = 9999;
	_uint				m_iCurrentFileIdx = 9999;
	wstring				m_CurSelectedTextureFilePath;

private:
	HRESULT	Save_Level();
	HRESULT	Save_UI(_uint iIndex);
	HRESULT Load_Levels();
	HRESULT	Load_UI();

private:
	void	Show_LevelTab();
	void	Show_UITab();
	void	Show_UIList();

private:
	void	Create_Ortho();
	void	Create_Perspective();

private:
	void	Show_ListBox();
	void	Show_Components(_uint iIndex);

	void	Show_Transform(_uint iIndex);
	void	Show_Physics(_uint iIndex);
	void	Show_Texture(_uint iIndex);
	void	Show_Shader(_uint iIndex);
	void	Show_Fader(_uint iIndex);

private:
	_uint	Check_KeyInput();

	void	Read_Folder(const char* pFolderPath);


};
END
