#pragma once

namespace Engine
{

	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum RENDER_ID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum GROUP_ID
	{
		GR_DEFAULT,
		GR_END = 32
	};

	enum COL_ID
	{
		COL_DEFAULT,
		COL_END = 32
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