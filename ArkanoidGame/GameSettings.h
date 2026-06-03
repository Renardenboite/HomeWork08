#pragma once
#include <string>

namespace ArkanoidGame
{
	class GameWorld
	{
	private:
		GameWorld() = default;

	public:
		static GameWorld& Instance()
		{
			static GameWorld gameWorld;
			return gameWorld;
		}

		const std::string RESOURCES_PATH = "Resources/";
		const std::string TEXTURES_PATH = RESOURCES_PATH + "Textures/";
		const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
		const std::string SOUNDS_PATH = RESOURCES_PATH + "Sounds/";
		const std::string LEVELS_CONFIG_PATH = RESOURCES_PATH + "levels.config";

		const unsigned int SCREEN_WIDTH = 800;
		const unsigned int SCREEN_HEIGHT = 600;
		const unsigned int BLOCK_SHIFT = 5;

		const float BLOCK_HEIGHT = 30.f;		        
        const float blockSpacingX = 15.f;
        const float blockSpacingY = 10.f;

        const float PLATFORM_WIDTH = 130.f;
        const float PLATFORM_HEIGHT = 20.f;
        const float PLATFORM_SPEED = 400.f;

        const float BALL_SIZE = 20.f;
        const float BALL_SPEED = 150.f;

		const float BONUS_SIZE = 50.f;

        const float ACCELERATION = 200.f;

        const int MAX_APPLES = 600;
        const float TIME_PER_FRAME = 1.f / 60.f;

        const int MAX_RECORDS_TABLE_SIZE = 5;
        const char* PLAYER_NAME = "Player";

        const float BREAK_DELAY = 0.8f;
        const float BONUS_DURATION = 30.f;
        const int BONUS_PROPABILITY_PERCENT = 10;

        const std::string GAME_NAME = "ArkanoidGame";
	};	
}

#define SETTINGS GameWorld::Instance()
