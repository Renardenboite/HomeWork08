#include "LevelLoader.h"
#include "GameSettings.h"

#include <fstream>
#include <assert.h>
#include <filesystem>

namespace ArkanoidGame
{
	Level& LevelLoader::GetLevel(int i) { return levels.at(i); }
	
	BlockType LevelLoader::CharToBlockType(char symbol)
	{
		BlockType blockType;
		switch (symbol)
		{
		case '1':
			blockType = BlockType::Simple;
			break;
		case '2':
			blockType = BlockType::ThreeHit;
			break;
		case '0':
			blockType = BlockType::Unbreackable;
			break;
		default:
			assert(false);
			break;
		}
		return blockType;
	}

    void LevelLoader::LoadLevelsFromFile() {
        std::ifstream file(SETTINGS.LEVELS_CONFIG_PATH);
        std::string line;
        Level currentLevel;
        bool inLevel = false;
        int y = 0;

        auto finalizeLevel = [&]() {
            if (inLevel && !currentLevel.m_blocks.empty()) {
                int maxX = 0, maxY = 0;
                for (auto& [pos, type] : currentLevel.m_blocks) {
                    maxX = std::max(maxX, pos.x);
                    maxY = std::max(maxY, pos.y);
                }
                currentLevel.width = maxX + 1;
                currentLevel.height = maxY + 1;
                levels.push_back(std::move(currentLevel));
            }
            currentLevel = Level{};
            inLevel = false;
            y = 0;
            };

        while (std::getline(file, line))
        {
            size_t start = line.find_first_not_of(" \t\r\n");
            size_t end = line.find_last_not_of(" \t\r\n");
            if (start == std::string::npos)
            {
                if (inLevel) finalizeLevel();
                continue;
            }
            std::string trimmed = line.substr(start, end - start + 1);

            if (trimmed.find("level ") == 0)
            {
                finalizeLevel();
                inLevel = true;
                y = 0;
            }
            else if (inLevel)
            {
                int x = 0;
                for (char c : trimmed)
                {
                    if (c != ' ') {
                        currentLevel.m_blocks.emplace_back(sf::Vector2i{ x, y }, CharToBlockType(c));
                    }
                    ++x;
                }
                ++y;
            }
        }
        finalizeLevel();
    }

	int LevelLoader::GetLevelCount()
	{
		return static_cast<int>(levels.size());
	}
}