#pragma once
namespace Client
{
	enum PROTOTYPE_ID_TYPE
	{
		ID_LEVEL,
		ID_GAMEOBJECT,
		ID_COMPONENT,
		ID_END
	};

	enum CHANNEL_CLIENT
	{
		CHANNEL_BGM,
		CHANNEL_PLAYER,
		CHANNEL_ENVIRONMENT,
		CHANNEL_EFFECTS,
		CHANNEL_VOICE,
		CHANNEL_END = CH_GROUP_END
	};

	enum OBJ_GROUP_CLIENT
	{
		GROUP_STATIC,
		GROUP_ENEMY,
		GROUP_DECORATION,
		GROUP_PROP,
		GROUP_UI,

		GROUP_END = GR_END
	};

	enum LEVEL_TYPE_CLIENT
	{
		LEVEL_UNITY,
		LEVEL_LOGO,
		LEVEL_MAINMENU,
		LEVEL_LOBBY,
		LEVEL_DUNGEON,
		LEVEL_END
	};
}

