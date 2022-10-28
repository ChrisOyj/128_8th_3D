#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CResource;

struct MODEL_DATA
{
	vector<wstring> m_vecResourceKeys;
	vector<RES_TYPE> m_vecResType;
};

class CResource_Manager
{
public:

	DECLARE_SINGLETON(CResource_Manager)

private:
	CResource_Manager();
	~CResource_Manager();

public:
	MODEL_DATA*	Get_ModelData(wstring wstrFilePath, MODEL_TYPE eType);
	CResource* Get_Resource(wstring wstrResourceKey);

private:
	Assimp::Importer			m_Importer;
	map<_hashcode, MODEL_DATA*>	m_mapModelData;
	map<_hashcode, CResource*>	m_mapResources;
	_uint						m_iRootNodeIdx = 0;

private:
	MODEL_DATA* Find_ModelData(wstring wstrFilePath);
	MODEL_DATA* Make_ModelData(wstring wstrFilePath, MODEL_TYPE eType); // Open dat

	MODEL_DATA* Open_DatFile(wstring wstrDatFilePath);
	MODEL_DATA* Create_DatFile(wstring wstrFilePath, MODEL_TYPE eType);

private:
	void		Save_Material(const aiMaterial* pMaterial, ofstream* pWriteFile, MODEL_TYPE eType);
	void		Save_Node(const aiNode* pNode, ofstream* pWriteFile);
	void		Save_Mesh(const aiMesh* pMesh, MODEL_TYPE eType, ofstream* pWriteFile, _uint iIdx);
	void		Save_Anim(const aiAnimation* pAnim, ofstream* pWriteFile);

private:
	CResource* Find_Resource(wstring strResourceKey);
	void Add_Resource(wstring strResourceKey, CResource*	pResource);

private:
	CResource* Create_Resource(ifstream* pReadFile, string strName, RES_TYPE eType);

private:
	HRESULT		Fill_ModelData(ifstream* pReadFile, RES_TYPE eType, MODEL_DATA* pOut);

private:
	void		Release();


};
END
