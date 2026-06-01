#pragma once
#include "Block.h"
#include "ThreeHitBlock.h"

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace ArkanoidGame
{
	enum class BlockType
	{
		Simple,
		ThreeHit,
		Unbreackable
	};

	struct Level
	{
		int width = 0;
		int height = 0;
		std::vector<std::pair<sf::Vector2i, BlockType>> m_blocks;
	};

	class LevelLoader final
	{
	public:
		LevelLoader() { LoadLevelsFromFile(); };
		Level& GetLevel(int i);
		~LevelLoader() = default;
		int GetLevelCount();
		bool HasLevels() const { return !levels.empty(); }

	private:
		void LoadLevelsFromFile();

		static BlockType CharToBlockType(char symbol);
		std::vector<Level> levels;
	};
}