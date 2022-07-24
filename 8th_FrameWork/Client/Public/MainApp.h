#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
	class CGameInstance;
END

BEGIN(Client)
//class CGameInstance;
class CMainApp final
{
	DECLARE_SINGLETON(CMainApp)

private:
	CMainApp();
	~CMainApp();

public:
	HRESULT Initialize();	
	HRESULT	Progress();

private:
	CGameInstance*			m_pGameInstance = nullptr;

private:
	HRESULT Render();
	void	Release();
};

END