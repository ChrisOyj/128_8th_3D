#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

enum TOOL_MAP_TYPE
{
	TOOL_MAP_DEFAULT,
	TOOL_MAP_GROUND,
	TOOL_MAP_PROP,
	TOOL_MAP_END
};

class CWindow_Map
	: public CImGui_Window
{
	struct TREE_DATA
	{
		string strFullPath;
		string strFileName;
		string strFolderPath;
		vector<TREE_DATA>	vecChildren;
	};

	struct MESH_ITEM
	{
		string strName;
		wstring wstrFilePath;
		TOOL_MAP_TYPE eClassType;
		CGameObject* pGameObject;
		_bool		bSelected;
	};

private:
	CWindow_Map();
	virtual ~CWindow_Map();

public:
	static CWindow_Map* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	char				m_szMapName[MIN_STR];
	char				m_szInstancingName[MIN_STR];

	_uint				m_iCurrentIdx = 9999;
	vector<MESH_ITEM>	m_vecCurMeshes;
	string				m_CurSelectedMeshFilePath;
	vector<string>		m_vecSelectedMeshFilePath;
	TREE_DATA			m_RootTreeNode;

	_bool				m_bCurMapType[TOOL_MAP_END];
	_bool				m_bProp = false;
	_bool				m_bInstancing = false;
	class CMap_Sample*		m_pSampleObject = nullptr;

	vector<VTXINSTANCE>	m_vecVtxInstance;

	vector<LIGHTDESC>	m_vecLights;
	_bool				m_bLight = false;
	LIGHTDESC			m_tCurLightDesc;

	_float				m_fInstancingTimeAcc = 0.f;


private:
	void		Show_MapList();
	void		Show_MapLoader();
	void		Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree);
	void		Show_TreeData(TREE_DATA& tTree);
	void		Change_Passes(VTXMODEL_PASS_TYPE eType);
	void		Create_Selected_Meshes(TOOL_MAP_TYPE eType, wstring pFilePath);

private:
	void		Save_CurMap();
	void		Save_InstancingData();
	void		Save_Lights();
	void		Load_InstancingData();

};
END

