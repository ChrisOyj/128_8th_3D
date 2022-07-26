#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CSound_Device
{
	DECLARE_SINGLETON(CSound_Device);

private:
	CSound_Device();
	~CSound_Device();

public:
	HRESULT		Initialize(const SOUNDDESC& tSoundDesc);
	HRESULT		Release();

public:
	void		Load_SoundFile();

	void		Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio = 1.f);
	void		Play_BGM(const _tchar* strSoundKey);

	void		Stop_Sound(CHANNEL_GROUP eType);
	void		Stop_All();

	void		Set_Volume(_float fVolume);
	void		Set_ChannelVolume(CHANNEL_GROUP eID, _float fVolume);
	void		Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume);

private:
	map <_hashcode, FMOD_SOUND*> m_mapSound;

	vector<FMOD_CHANNEL*>	m_pChannelArr[CH_GROUP_END];
	_uint					m_iChannelNumbers[CH_GROUP_END];

	_float					m_fChannelVolume[CH_GROUP_END];

	FMOD_SYSTEM* m_pSystem = nullptr;

	_uint					m_iCurChannelIndex[CH_GROUP_END];
};

END