#pragma once
namespace Client
{
	struct FADEDESC
	{
		enum FADE_STYLE
		{
			FADE_STYLE_DEFAULT,
			FADE_STYLE_MOVE,
			FADE_STYLE_SCALEUP, // 원본보다 크기가 위
			FADE_STYLE_SCALEDOWN, // 원본보다 크기가 작음
			FADE_STYLE_END,
		};

		enum FADEOUT_TYPE
		{
			FADEOUT_DELETE,
			FADEOUT_DISABLE,
			FADEOUT_NEXTTEXTURE,
			FADEOUT_RANDOMTEXTURE,
			FADEOUT_NONE,
			FADEOUT_END,
		};

		FADE_STYLE		eFadeStyle = FADE_STYLE_END;
		FADEOUT_TYPE	eFadeOutType = FADEOUT_END;
		_byte		bFadeInFlag = 0;
		_byte		bFadeOutFlag = 0;
		_float		fFadeInStartTime = 0.f;
		_float		fFadeInTime = 0.f;
		_float		fFadeOutTime = 0.f;
		_float		fFadeOutStartTime = 0.f;
		_float		fAlpha = 0.f;
		KEY			eKeyType = KEY::LAST;
	};



	struct UNIT_STATUS
	{
		_float fHP = 100.f;
		_float fMaxHP = 100.f;
		_float fRunSpeed = 7.5f;
		_float fShortDashSpeed = 11.f;
		_float fWalkSpeed = 2.f;
		_float fDashSpeed = 16.f;
		_float fWallJumpSpeed = 15.f;
		_float fWireSpeed = 25.f;
		_float fJumpPower = 6.5f;
	};

	struct UNIT_MODEL_DATA
	{
		wstring	strModelPaths[MODEL_PART_END];
		string	strRefBoneName[MODEL_PART_END];
		_uint	iEyeIndex = 0;
		_bool	bOverall = false;
		ANIM_TYPE	eStartPose = ANIM_WIN_TYPE_4;
	};
}

