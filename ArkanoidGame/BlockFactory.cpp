#include "BlockFactory.h"
#include "Block.h"
#include "ThreeHitBlock.h"

namespace ArkanoidGame
{
	int BlockFactory::GetcreatedBreackableBlocksCount() const
	{
		return createdBreackableBlocksCount;
	}

	void BlockFactory::ClearCounter()
	{
		createdBreackableBlocksCount = 0;
	}

	std::shared_ptr<Block> SimpleBlockFactory::CreateBlock(const sf::Vector2f& position, float width, float height)
	{
		++createdBreackableBlocksCount;
		return std::make_shared<SmoothDestroyableBlock>(position, width, height);
	}

	std::shared_ptr<Block> ThreeHitBlockFactory::CreateBlock(const sf::Vector2f& position, float width, float height)
	{
		++createdBreackableBlocksCount;
		return std::make_shared<ThreeHitBlock>(position, width, height);
	}

	std::shared_ptr<Block> UnbreackableBlockFactory::CreateBlock(const sf::Vector2f& position, float width, float height)
	{		
		return std::make_shared<UnbreackableBlock>(position, width, height);
	}
}