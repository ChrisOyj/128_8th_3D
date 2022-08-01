#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract
{
	friend class CLevel_Manager;

protected:
	CLevel();
	virtual ~CLevel();

public:
	void	Destroy_Instance() { delete this; }

public:
	//virtual void Load() PURE; // ���� ������ �ҷ��ͼ� Prototype �̸� ä������
	virtual HRESULT	Enter() PURE;
	virtual void	Tick() PURE;
	virtual void	Late_Tick() PURE;
	virtual HRESULT Render() PURE;
	virtual HRESULT	Exit() PURE;

private:

};

END