#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CModel;
END

BEGIN(Client)
class CUnit;
class CEffect;
class CWindow_Effect
	: public CImGui_Window
{
	struct EFFECT_ITEM
	{
		string	strName;
		CEffect* pEffect = nullptr;
		_bool bSelected = false;
		_uint	iEffectType = 0;

	};

	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};



private:
	CWindow_Effect();
	virtual ~CWindow_Effect();

public:
	static CWindow_Effect* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	char				m_szCurEffectName[MIN_STR];
	vector<EFFECT_ITEM>	m_vecEffects;
	_uint				m_iCurrentIdx = 9999;

	TREE_DATA			m_MeshRootNode;
	TREE_DATA			m_TextureRootNode;
	TREE_DATA			m_EffectRootNode;


	string				m_CurSelectedMeshFilePath;
	string				m_CurSelectedTextureFilePath;
	string				m_CurSelectedEffectFileKey;

private:
	void	Show_MainList();
	void	Show_EffectTab();
	void	Show_ParticleTab();

	
private:
	void	Save_CurEffect();
	void	Load_SelectedEffect();


private:
	void		Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void		Show_TreeData(TREE_DATA& tTree);
	void		Show_TreeData_Texture(TREE_DATA& tTree);
	void		Show_TreeData_Effect(TREE_DATA& tTree);
	void		Change_Model(CModel* pModelCom);
	void		Change_Model(CModel* pModelCom, _float4x4 transformMatrix);

};
END

