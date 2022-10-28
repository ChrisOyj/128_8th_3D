#include "..\Public\CLight_Manager.h"
#include "CLight.h"
#include "GameInstance.h"
#include "CMesh_Rect.h"
#include "CShader.h"

#include "GameObject.h"
#include "Transform.h"
IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

CLight_Manager::~CLight_Manager()
{
	Release();
}

const LIGHTDESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(const LIGHTDESC& LightDesc)
{
	m_ReservedLights.push_back(LightDesc);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader* pShader, CMesh_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

void CLight_Manager::Clear_Lights()
{
	Release();
}

void CLight_Manager::Pop_Light()
{
	SAFE_DELETE(m_Lights.back());
	m_Lights.pop_back();
}

HRESULT CLight_Manager::Load_Lights(wstring wstrPath)
{
	wstring wstrFilePath = L"../bin/Lights/";
	wstrFilePath += wstrPath;
	wstrFilePath += L".bin";

	ifstream	readFile(wstrFilePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Failed to wrtie File : CWindow_Map");
		return E_FAIL;
	}


	_uint iNumLights = 0;
	readFile.read((char*)&iNumLights, sizeof(_uint));

	for (_uint i = 0; i < iNumLights; ++i)
	{
		LIGHTDESC	tLightDesc;
		readFile.read((char*)&tLightDesc, sizeof(LIGHTDESC));
		tLightDesc.eType = LIGHTDESC::TYPE_POINT;
		if (FAILED(Add_Light(tLightDesc)))
			return E_FAIL;
	}

	readFile.close();

	return S_OK;
}

void CLight_Manager::Update_Lights()
{
	

	for (auto iter = m_Lights.begin(); iter != m_Lights.end();)
	{
		CLight* pLight = *iter;

		if (pLight->m_LightDesc.pOwner)
		{
			pLight->m_LightDesc.vPosition = pLight->m_LightDesc.pOwner->Get_Transform()->Get_World(WORLD_POS) + pLight->m_LightDesc.vOffset;


			pLight->m_LightDesc.fLightAcc += fDT;

			if (pLight->m_LightDesc.fLightAcc > pLight->m_LightDesc.fLightTime)
			{
				SAFE_DELETE(pLight);
				iter = m_Lights.erase(iter);
				continue;
			}
			else
			{
				if (!pLight->m_LightDesc.pOwner->Is_Valid())
				{
					SAFE_DELETE(pLight);
					iter = m_Lights.erase(iter);
					continue;
				}
				else
				{
					++iter;
					continue;
				}
			}

			
		}
		else
		{
			pLight->m_LightDesc.fLightAcc += fDT;

			if (pLight->m_LightDesc.fLightAcc > pLight->m_LightDesc.fLightTime)
			{
				SAFE_DELETE(pLight);
				iter = m_Lights.erase(iter);
				continue;
			}
			else
			{
				++iter;
				continue;
			}
		}
			

	}

	for (auto& elem : m_ReservedLights)
	{
		CLight* pLight = CLight::Create(elem);

		m_Lights.push_back(pLight);
	}

	m_ReservedLights.clear();
}

void CLight_Manager::Release()
{
	for (auto& pLight : m_Lights)
		SAFE_DELETE(pLight);

	m_Lights.clear();
}
