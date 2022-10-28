#pragma once
#include "CEffect.h"

BEGIN(Client)
class CScreenEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CScreenEffect);

public:
	struct INSTANCING_DATA
	{
		enum FADETYPE
		{
			FADEINREADY,
			FADEIN,
			FADEOUTREADY,
			FADEOUT,
			FADE_END,
		};
		FADETYPE	eCurFadeType = FADEINREADY;


		_bool	bAlive = true;

		_float	fTimeAcc = 0.f;
		_float2	vCreatePos; // �� ���� ������ ����
		_float	fDistance = 0.f; // �� ���� ���� ����
		_float	fFadeInStartTime; // ���� ������
		_float	fFadeTime;	//�����ǰ� ������� ���� �ð�
		_float	fFadeOutStartTime;
		_float	fRadius = 0.f; // ����

		//Lerp��
		_float	fOriginDistance = 0.f;
		_float	fTargetDistance = 0.f;


	};

protected:
	CScreenEffect();
	CScreenEffect(const CScreenEffect& _origin);
	virtual ~CScreenEffect();

public:
	static CScreenEffect* Create(_uint iNumInstance);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

protected:
	virtual void	My_Tick() override;
	virtual void	My_LateTick() override;
	virtual void	OnEnable() override;

private:
	_uint			m_iNumInstance;
	VTXTRIINSTANCE* m_pTriInstances = nullptr;

	//������ �ð�, ������� �ð� ����
	INSTANCING_DATA* m_pInstancingDatas = nullptr;

	_uint		m_iNumDeads = 0;


private:
	virtual void	Dead_Instance(_uint iIndex);

};

END