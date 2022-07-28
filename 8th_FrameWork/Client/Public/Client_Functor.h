#pragma once

namespace Client
{
	static void Call_MsgBox_Index(const _tchar* strMessage, const _uint& iIdx)
	{
#ifdef _DEBUG

		wstring strFullMsg = strMessage;
		strFullMsg += L" (Index : ";
		strFullMsg += to_wstring(iIdx);
		strFullMsg += L")";

		Engine::Call_MsgBox(strFullMsg.c_str());

#endif // _DEBUG
	}

	static _bool	Safe_CheckID(const _uint& iID, PROTOTYPE_ID_TYPE	eIDType)
	{
		_uint minID = 0;
		_uint maxID = 0;

		switch (eIDType)
		{
		case Client::ID_LEVEL:

			minID = g_iMinLevelID;
			maxID = g_iMaxLevelID;
			break;

		case Client::ID_GAMEOBJECT:
			minID = g_iMinGameObjectID;
			maxID = g_iMaxGameObjectID;
			break;

		case Client::ID_COMPONENT:
			minID = g_iMinComponentID;
			maxID = g_iMaxComponentID;
			break;

		default:
			break;
		}

		if (minID >= iID || maxID <= iID)
			return false;

		return true;
	}
}