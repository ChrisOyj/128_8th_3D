#pragma once
#include "CImGui_Window.h"


BEGIN(Client)

class CUI;

class CWindow_UI final
	: public CImGui_Window
{
public:
	struct UI_ITEM
	{
		CUI* pUI = nullptr;
		_uint iGameObjectID = 0;
		_bool bSelected = false;
		_bool bOrtho = false;

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
	vector<UI_ITEM>		m_vecUI;
	_uint				m_iCurrentIdx = 9999;
	_uint				m_iUIID = 100;

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


};
END
