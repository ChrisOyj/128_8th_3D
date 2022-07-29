#pragma once

namespace Engine
{

	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum RENDER_GROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum OBJ_GROUP
	{
		GR_DEFAULT,
		GR_END = 32
	};

	enum COL_GROUP
	{
		COL_DEFAULT,
		COL_END = 32
	};

	enum COM_GROUP
	{
		COM_GROUP_DEFAULT,
		COM_GROUP_END = 32
	};

#pragma region CHANNEL_ENUM

	enum CHANNEL_GROUP
	{
		CH_BGM,
		CH_GROUP_0,
		CH_GROUP_1,
		CH_GROUP_2,
		CH_GROUP_3,
		CH_GROUP_END = 5
	};


#pragma endregion

}