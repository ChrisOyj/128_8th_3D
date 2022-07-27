#pragma once
#include "Engine_Defines.h"

//각 Level은 해당 레벨에서 생성할 오브젝트에 대한 정보를 가지고 있다. (Json과 연동)
//Render는 Render Manager에서 진행.

BEGIN(Engine)

class ENGINE_DLL CLevel abstract
{
protected:
	CLevel();
	virtual ~CLevel();

public:
	//virtual void Load() PURE; // 파일 연동시 불러와서 Prototype 미리 채워놓기

	virtual void	Enter() PURE;
	virtual void	Tick() PURE;
	virtual void	Late_Tick() {}
	virtual HRESULT Render() PURE;
	virtual void	Exit() PURE;

private:
	friend class CLevel_Manager;
	void	Destory_Instance() { delete this; }

};

END