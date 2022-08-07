#pragma once
namespace Client
{
	struct FADEDESC
	{
		enum FADEOUT_TYPE
		{
			FADEOUT_DELETE,
			FADEOUT_DISABLE,
			FADEOUT_NEXTTEXTURE,
			FADEOUT_RANDOMTEXTURE,
			FADEOUT_END,
		};

		FADEOUT_TYPE	eFadeOutType = FADEOUT_END;
		_byte		bFadeFlag = 0;
		_float		fFadeInTime = 0.f;
		_float		fFadeOutTime = 0.f;
		_float		fFadeOutStartTime = 0.f;
		_float		fAlpha = 0.f;
		KEY			eKeyType = KEY::LAST;
	};
}

