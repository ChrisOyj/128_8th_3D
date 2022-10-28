#include "CUtility_File.h"

HRESULT CUtility_File::Write_Text(ofstream* pWriteFile, const char* pText)
{
	_uint iSize = strlen(pText);

	pWriteFile->write((char*)&iSize, sizeof(_uint));
	pWriteFile->write(pText, iSize);

    return S_OK;
}

string CUtility_File::Read_Text(ifstream* pReadFile)
{
	char szBuff[MAX_STR] = "";
	_uint iSize = 0;
	pReadFile->read((char*)&iSize, sizeof(_uint));
	pReadFile->read(szBuff, iSize);

	return string(szBuff);
}
