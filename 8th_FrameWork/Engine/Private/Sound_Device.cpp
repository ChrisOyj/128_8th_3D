#include "../Public/Sound_Device.h"

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
	}

	Set_Volume(0.5f);


	Load_SoundFile();

	return S_OK;
}

HRESULT CSound_Device::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_RESULT fHr = FMOD_Sound_Release(Mypair.second);
	}

	m_mapSound.clear();

	FMOD_RESULT fHr = FMOD_System_Release(m_pSystem);
	fHr = FMOD_System_Close(m_pSystem);

	return S_OK;
}


void CSound_Device::Load_SoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;
	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	INT_PTR handle = _findfirst("../Bin/Resources/Sound/GamePlay/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Resources/Sound/GamePlay/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			_hashcode hcCode = Convert_ToHash(pSoundKey);

			m_mapSound.emplace(hcCode, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}


	_findclose(handle);

	{
		// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
		_finddata_t fd;
		// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
		INT_PTR handle = _findfirst("../Bin/Resources/Sound/UI/*.*", &fd);

		if (handle == -1)
			return;

		int iResult = 0;

		char szCurPath[128] = "../Bin/Resources/Sound/UI/";
		char szFullPath[128] = "";

		while (iResult != -1)
		{
			strcpy_s(szFullPath, szCurPath);

			// "../ Sound/Success.wav"
			strcat_s(szFullPath, fd.name);

			//wstring strFilePath = CPathMgr::Get_Instance()->Get_ContentPath();
			//strFilePath += fd.name;

			FMOD_SOUND* pSound = nullptr;

			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				int iLength = (int)strlen(fd.name) + 1;

				TCHAR* pSoundKey = new TCHAR[iLength];
				ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

				// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
				MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

				_hashcode hcCode = Convert_ToHash(pSoundKey);

				m_mapSound.emplace(hcCode, pSound);
			}
			//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
			iResult = _findnext(handle, &fd);
		}

		_findclose(handle);

	}

	FMOD_System_Update(m_pSystem);

}

void CSound_Device::Play_BGM(const TCHAR* strSoundKey)
{
	map<_hashcode, FMOD_SOUND*>::iterator iter;

	iter = m_mapSound.find(Convert_ToHash(strSoundKey));

	if (iter == m_mapSound.end())
	{
		Call_MsgBox(L"Failed to find SoundKey : CSound_Device");
		return;
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[CH_BGM].front());
	FMOD_Channel_SetMode(m_pChannelArr[CH_BGM].front(), FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSound_Device::Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	map<_hashcode, FMOD_SOUND*>::iterator iter;

	iter = m_mapSound.find(Convert_ToHash(strSoundKey));

	if (iter == m_mapSound.end())
	{
		Call_MsgBox(L"Failed to find SoundKey : CSound_Device");
		return;
	}

	FMOD_BOOL bPlay = FALSE;

	_uint iCurChannel = m_iCurChannelIndex[iGroupIndex];

	if (FMOD_Channel_IsPlaying(m_pChannelArr[iGroupIndex][iCurChannel], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[iGroupIndex][iCurChannel]);
		Set_ChannelVolume(iGroupIndex, iCurChannel, m_fChannelVolume[iGroupIndex] * fVolumeRatio);
	}

	++m_iCurChannelIndex[iGroupIndex];

	if (m_iCurChannelIndex[iGroupIndex] >= m_iChannelNumbers[iGroupIndex])
		m_iCurChannelIndex[iGroupIndex] = 0;
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
