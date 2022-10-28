#include "CResource_Animation.h"

#include "CUtility_Json.h"
#include "CUtility_File.h"

#include "Channel.h"

CResource_Animation::CResource_Animation()
{
}

CResource_Animation::~CResource_Animation()
{
	for (_uint i = 0; i < m_tAnimDesc.iNumChannels; ++i)
	{
		SAFE_DELETE_ARRAY(m_tAnimDesc.pChannels[i].pKeyFrames);
	}

	SAFE_DELETE_ARRAY(m_tAnimDesc.pChannels);
}

CResource_Animation* CResource_Animation::Create(ifstream* pReadFile, string strName)
{
	CResource_Animation* pResource = new CResource_Animation;

	if (FAILED(pResource->Initialize(pReadFile, strName)))
	{
		Call_MsgBox(L"Failed to Initiailize : CResource_Animation");
		SAFE_DELETE(pResource);
	}

	return pResource;
}

HRESULT CResource_Animation::Initialize(ifstream* pReadFile, string strName)
{
	__super::Initialize(pReadFile, strName);
	
	pReadFile->read((char*)&m_tAnimDesc.fDuration, sizeof(_float));
	pReadFile->read((char*)&m_tAnimDesc.fTickPerSecond, sizeof(_float));
	pReadFile->read((char*)&m_tAnimDesc.iNumChannels, sizeof(_uint));

	m_tAnimDesc.pChannels = new CHANNEL_DESC[m_tAnimDesc.iNumChannels];

	if (strName == "CustomMan_DoubleJump")
	{
		m_tAnimDesc.fDuration *= 0.7674f;
		m_tAnimDesc.fTickPerSecond *= 2.f;

		//키프레임 새로만들어야함
		for (_uint i = 0; i < m_tAnimDesc.iNumChannels; ++i)
		{
			m_tAnimDesc.pChannels[i].strName = CUtility_File::Read_Text(pReadFile);
			pReadFile->read((char*)&m_tAnimDesc.pChannels[i].iNumKeyFrames, sizeof(_uint));

			KEYFRAME* pTemp = new KEYFRAME[m_tAnimDesc.pChannels[i].iNumKeyFrames];
			pReadFile->read((char*)pTemp, sizeof(KEYFRAME) * m_tAnimDesc.pChannels[i].iNumKeyFrames);

			m_tAnimDesc.pChannels[i].iNumKeyFrames -= 10;
			m_tAnimDesc.pChannels[i].pKeyFrames = new KEYFRAME[m_tAnimDesc.pChannels[i].iNumKeyFrames];

			for (_uint j = 0; j < m_tAnimDesc.pChannels[i].iNumKeyFrames; ++j)
			{
				m_tAnimDesc.pChannels[i].pKeyFrames[j] = pTemp[j + 10];
				m_tAnimDesc.pChannels[i].pKeyFrames[j].fTime = pTemp[j].fTime;
			}

			delete[] pTemp;
		}

		return S_OK;
	}

	for (_uint i = 0; i < m_tAnimDesc.iNumChannels; ++i)
	{
		m_tAnimDesc.pChannels[i].strName = CUtility_File::Read_Text(pReadFile);
		pReadFile->read((char*)&m_tAnimDesc.pChannels[i].iNumKeyFrames, sizeof(_uint));

		m_tAnimDesc.pChannels[i].pKeyFrames = new KEYFRAME[m_tAnimDesc.pChannels[i].iNumKeyFrames];

		pReadFile->read((char*)m_tAnimDesc.pChannels[i].pKeyFrames, sizeof(KEYFRAME) * m_tAnimDesc.pChannels[i].iNumKeyFrames);
	}

	


	return S_OK;
}
