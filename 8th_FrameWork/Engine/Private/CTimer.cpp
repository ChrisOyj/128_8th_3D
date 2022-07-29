#include "CTimer.h"
#include "GameInstance.h"

#include "GameObject.h"

CTimer::CTimer(CGameObject* pOwner)
	: CComponent(pOwner)
{
}

CTimer::~CTimer()
{
}

CTimer* CTimer::Create(CGameObject* pOwner)
{
	CTimer* pTimer = new CTimer(pOwner);

	if (FAILED(pTimer->Initialize_Prototype()))
	{
		SAFE_DELETE(pTimer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTimer");
		return nullptr;
	}

	return pTimer;
}

void CTimer::Add_Timer(const _float& fTime, const _uint& iEventNum, _bool bLoop)
{
	TIMER_EVENT	tTimerEvent;
	tTimerEvent.fOriginTime = fTime;
	tTimerEvent.fCurTime = tTimerEvent.fOriginTime;
	tTimerEvent.iEventNum = iEventNum;
	tTimerEvent.bLoop = bLoop;

	m_TimerEvents.push_back(tTimerEvent);
}

HRESULT CTimer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTimer::Initialize()
{
	return S_OK;
}

void CTimer::Tick()
{
	for (auto TimerIter = m_TimerEvents.begin(); TimerIter != m_TimerEvents.end();)
	{
		TimerIter->fCurTime -= fDT;

		if (TimerIter->fCurTime <= 0.f)
		{
			m_pOwner->CallBack_TimerEvent(TimerIter->iEventNum);

			if (TimerIter->bLoop)
			{
				TimerIter->fCurTime = TimerIter->fOriginTime;
				++TimerIter;
				continue;
			}
			else
			{
				TimerIter = m_TimerEvents.erase(TimerIter);
				continue;
			}
		}

		++TimerIter;
	}
}

void CTimer::Late_Tick()
{
}

void CTimer::OnEnable()
{
	__super::OnEnable();
}

void CTimer::OnDisable()
{
	__super::OnDisable();

	/* Delete all timers*/
	m_TimerEvents.clear();
}

void CTimer::Release()
{
}