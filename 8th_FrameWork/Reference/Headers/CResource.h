#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class CResource
{
protected:
	CResource();
	virtual ~CResource();

	friend class CResource_Manager;

public:
	string	Get_Name() { return m_strName; }
	
public:
	virtual HRESULT	Initialize(ifstream* pReadFile, string strName);


protected:
	string		m_strName;

};
END
