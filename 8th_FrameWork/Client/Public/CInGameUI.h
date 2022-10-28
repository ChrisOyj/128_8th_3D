#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CDefault_UI;

class CInGameUI
	: public CGameObject
{
	DECLARE_PROTOTYPE(CInGameUI);
	friend class CUser;

private:
	CInGameUI();
	virtual ~CInGameUI();

public:
	static CInGameUI* Create();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	void	Disable_HUD();
	void	Enable_HUD();

private:
	//�ΰ��� UI ����
	// 1. �޺�, KO, Ŭ���� ��
	class CUI_Combo* m_pUI_Combo = nullptr;
	// 2. ü�¹�
	class CUI_HUD* m_pUI_HUD = nullptr;
	// 3. ��ųâ
	class CUI_Skill* m_pUI_Skill = nullptr;
	// 4. �ð�
	class CUI_Time* m_pUI_Time = nullptr;
	// 5. ����â
	class CUI_Intro* m_pUI_Intro = nullptr;


private:
	virtual void My_Tick() override;

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;


};
END

