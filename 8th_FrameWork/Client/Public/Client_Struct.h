#pragma once
namespace Client
{
	struct FADEDESC
	{
		_byte		bFadeFlag = 0;
		_float		fFadeInTime = 0.f;
		_float		fFadeOutTime = 0.f;
		_float		fFadeOutStartTime = 0.f;
		_float		fAlpha = 0.f;
		KEY			eKeyType = KEY::LAST;
	};
}

