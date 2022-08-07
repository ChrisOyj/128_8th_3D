#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

struct GameObejctData
{
	_uint	iPrototypeID = 0;
	OBJ_GROUP_CLIENT	eGroupType = GROUP_END;
	_float4 vStartPosition;
	_float4 vStartLook;
};


class CLevel_Default
	: public CLevel
{
private:
	CLevel_Default();
	virtual ~CLevel_Default();

public:
	static CLevel_Default* Create(LEVEL_TYPE_CLIENT	eLevelID);

public:
	_float	Get_LoadingFinish() { return m_fLoadingFinish; }

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;

private:
	LEVEL_TYPE_CLIENT		m_eLevelType = LEVEL_END;
	list<GameObejctData>	m_tGameObjectDataList;

	_float					m_fLoadingFinish = 0.f;

private:
	HRESULT				Create_GameObjectInLevel(const GameObejctData& tData);
};

END