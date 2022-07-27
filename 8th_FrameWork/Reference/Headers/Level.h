#pragma once
#include "Engine_Defines.h"

//�� Level�� �ش� �������� ������ ������Ʈ�� ���� ������ ������ �ִ�. (Json�� ����)
//Render�� Render Manager���� ����.

BEGIN(Engine)

class ENGINE_DLL CLevel abstract
{
protected:
	CLevel();
	virtual ~CLevel();

public:
	//virtual void Load() PURE; // ���� ������ �ҷ��ͼ� Prototype �̸� ä������

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