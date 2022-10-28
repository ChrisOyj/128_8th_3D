#include "../Public/Sound_Device.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CSound_Device)


CSound_Device::CSound_Device()
{
	ZeroMemory(&m_iCurChannelIndex, sizeof(_uint) * CH_GROUP_END);
}

CSound_Device::~CSound_Device()
{
	Release();
}

HRESULT CSound_Device::Initialize(const SOUNDDESC& tSoundDesc)
{
	FMOD_System_Create(&m_pSystem);

	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	for (_uint i = 0; i < CH_GROUP_END; ++i)
	{
		m_pChannelArr[i].reserve(tSoundDesc.iChannelNumbers[i]);

		for (_uint j = 0; j < tSoundDesc.iChannelNumbers[i]; ++j)
		{
			m_pChannelArr[i].push_back(nullptr);
		}
		m_iChannelNumbers[i] = tSoundDesc.iChannelNumbers[i];
	}

	Set_Volume(0.5f);

	return S_OK;
}

HRESULT CSound_Device::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		for (auto& elem : Mypair.second)
		{
			FMOD_RESULT fHr = FMOD_Sound_Release(elem);
		}
	}

	m_mapSound.clear();

	FMOD_RESULT fHr = FMOD_System_Release(m_pSystem);
	fHr = FMOD_System_Close(m_pSystem);

	return S_OK;
}


HRESULT CSound_Device::Load_SoundFile(wstring wstrFolderPath)
{
	//1. 폴더를 읽고
	//2. 그안에 머 나오면 뒤에서 _이거찾아서 잘라서 저장
	//3. 

	for (filesystem::directory_iterator FileIter(wstrFolderPath.c_str());
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		iFind = (_int)strFileName.rfind("_");
		strFileName = strFileName.substr(0, iFind);

		wstring wstrFileName(strFileName.begin(), strFileName.end());

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, strFullPath.c_str(), FMOD_HARDWARE, 0, &pSound);

		m_mapSound[Convert_ToHash(wstrFileName)].push_back(pSound);
	}

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

void CSound_Device::Play_BGM(const TCHAR* strSoundKey)
{
	auto iter = m_mapSound.find(Convert_ToHash(strSoundKey));

	if (iter == m_mapSound.end())
	{
		Call_MsgBox(L"Failed to find SoundKey : CSound_Device");
		return;
	}

	FMOD_SOUND* pSound = iter->second.front();

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, pSound, FALSE, &m_pChannelArr[CH_BGM].front());
	FMOD_Channel_SetMode(m_pChannelArr[CH_BGM].front(), FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);

	iter->second.pop_front();
	iter->second.push_back(pSound);
}

void CSound_Device::Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{

	auto iter = m_mapSound.find(Convert_ToHash(strSoundKey));

	if (iter == m_mapSound.end())
	{
		Call_MsgBox(L"Failed to find SoundKey : CSound_Device");
		return;
	}

	FMOD_BOOL bPlay = FALSE;

	_uint iCurChannel = m_iCurChannelIndex[iGroupIndex];

	FMOD_SOUND* pSound = iter->second.front();


	//if (FMOD_Channel_IsPlaying(m_pChannelArr[iGroupIndex][iCurChannel], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, pSound, FALSE, &m_pChannelArr[iGroupIndex][iCurChannel]);
		Set_ChannelVolume(iGroupIndex, iCurChannel, m_fChannelVolume[iGroupIndex] * fVolumeRatio);
	}

	++m_iCurChannelIndex[iGroupIndex];

	if (m_iCurChannelIndex[iGroupIndex] >= m_iChannelNumbers[iGroupIndex])
		m_iCurChannelIndex[iGroupIndex] = 0;


	iter->second.pop_front();
	iter->second.push_back(pSound);
}

void CSound_Device::Stop_Sound(CHANNEL_GROUP eType)
{
	for (_uint i = 0; i < m_iChannelNumbers[eType]; ++ i)
		FMOD_Channel_Stop(m_pChannelArr[eType][i]);

	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Stop_All()
{
	for (_uint i = 0; i < CH_GROUP_END; ++i)
	{
		for (_uint j = 0; j < m_iChannelNumbers[i]; ++j)
		{
			FMOD_Channel_Stop(m_pChannelArr[i][j]);
		}
	}

	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Set_Volume(_float fVolume)
{
	for (_uint i = 0; i < CH_GROUP_END; ++i)
	{
		m_fChannelVolume[i] = fVolume;

		for (_uint j = 0; j < m_iChannelNumbers[i]; ++j)
		{
			FMOD_Channel_SetVolume(m_pChannelArr[i][j], fVolume);
		}
	}

	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Set_ChannelVolume(CHANNEL_GROUP eID, _float fVolume)
{
	m_fChannelVolume[eID] = fVolume;



	for (_uint i = 0; i < m_iChannelNumbers[eID]; ++i)
	{
		FMOD_Channel_SetVolume(m_pChannelArr[eID][i], fVolume);
	}

	FMOD_System_Update(m_pSystem);

}

void CSound_Device::Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID][iChannelIndex], fVolume);
	FMOD_System_Update(m_pSystem);

}
