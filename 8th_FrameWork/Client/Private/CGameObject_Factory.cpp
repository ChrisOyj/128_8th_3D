#include "stdafx.h"
#include "CGameObject_Factory.h"

#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"

#include "GameObject.h"

#include "CEmpty_GameObject.h"
#include "CDefault_UI.h"

#include "Functor.h"

CGameObject* CGameObject_Factory::Clone_GameObject(_hashcode hcClassName)
{
	CGameObject* pGameObject = CGameInstance::Get_Instance()->Clone_GameObject(hcClassName);

	if (!pGameObject)
	{
		return nullptr;
	}

	if (FAILED(pGameObject->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CGameObject_Factory");
		return nullptr;
	}

	return pGameObject;
}

CDefault_UI* CGameObject_Factory::Create_UI(wstring strFileKey)
{
	CDefault_UI* pDefaultUI = CDefault_UI::Clone_WithJson(strFileKey);

	return pDefaultUI;
}

vector<CGameObject*> CGameObject_Factory::Create_vecUI(wstring strFileTagKey)
{
	vector<CGameObject*> vecUI;

	for (filesystem::directory_iterator FileIter("../bin/Json");
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		strFileName = strFileName.substr(0, iFind2);

		_int iFind3 = (_int)strFileName.find("_");
		string strFileKey = strFileName.substr(0, iFind3);

		wstring wstrFileKey = CFunctor::To_Wstring(strFileKey);

		if (wstrFileKey != strFileTagKey)
			continue;

		vecUI.push_back(CDefault_UI::Clone_WithJson(CFunctor::To_Wstring(strFileName)));
	}

	return vecUI;
}

vector<CDefault_UI*> CGameObject_Factory::Create_vecUI_Reverse(wstring strFileTagKey)
{
	vector<CDefault_UI*> vecUI;

	for (filesystem::directory_iterator FileIter("../bin/Json");
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		strFileName = strFileName.substr(0, iFind2);

		_int iFind3 = (_int)strFileName.rfind("_");
		string strFileKey = strFileName.substr(0, iFind3);

		wstring wstrFileKey = CFunctor::To_Wstring(strFileKey);

		if (wstrFileKey != strFileTagKey)
			continue;

		vecUI.push_back(CDefault_UI::Clone_WithJson(CFunctor::To_Wstring(strFileName)));
	}

	return vecUI;
}
