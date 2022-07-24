#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

__interface ENGINE_DLL ITimerEvent
{
	void On_TimerEvent(int _iEventNum) PURE;
};

END