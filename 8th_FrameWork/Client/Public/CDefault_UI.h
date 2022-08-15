#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CDefault_UI
	: public CGameObject
{
	DECLARE_PROTOTYPE(CDefault_UI);
	DECLARE_GAMEOBJECT(CDefault_UI);

protected:
	CDefault_UI();
	virtual ~CDefault_UI();

public:
	static CDefault_UI* Clone_WithJson(wstring strFileKey);

public:
	wstring	Get_FileKey() { return m_strFileKey; }
	void Set_FileKey(wstring strFileKey) { m_strFileKey = strFileKey; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	wstring	m_strFileKey;
};

END