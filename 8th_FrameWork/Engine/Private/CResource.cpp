#include "CResource.h"

#include "CUtility_File.h"

CResource::CResource()
{
}

CResource::~CResource()
{
}

HRESULT CResource::Initialize(ifstream* pReadFile, string strName)
{
	m_strName = strName;

	return S_OK;
}
