#pragma once
#include "Engine_Defines.h"

class ENGINE_DLL CUtility_File
{
public:
	static HRESULT	Write_Text(ofstream* pWriteFile, const char* pText);
	static string	Read_Text(ifstream* pReadFile);
};

