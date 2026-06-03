#include "BonusFactory.h"
#include "WidePlatformEffect.h"
#include "SlowBallEffect.h"
#include "ExtraBallEffect.h"
#include <cstdlib>

namespace ArkanoidGame
{
    std::unique_ptr<Bonus> BonusFactory::CreateRandom(const sf::Vector2f& position)
    {
        int r = rand() % 3;
        switch (r)
        {
        case 0:
            return std::make_unique<Bonus>(position, std::make_unique<WidePlatformEffect>());
        case 1:
            return std::make_unique<Bonus>(position, std::make_unique<SlowBallEffect>());
        case 2:
            return std::make_unique<Bonus>(position, std::make_unique<ExtraBallEffect>());
        default:
            return std::make_unique<Bonus>(position, std::make_unique<WidePlatformEffect>());
        }
    }

    std::unique_ptr<Bonus> BonusFactory::Create(BonusType type, const sf::Vector2f& pos)
    {
        switch (type)
        {
        case BonusType::WidePlatform:
            return std::make_unique<Bonus>(pos, std::make_unique<WidePlatformEffect>());
        case BonusType::SlowBall:
            return std::make_unique<Bonus>(pos, std::make_unique<SlowBallEffect>());
        case BonusType::ExtraBall:
            return std::make_unique<Bonus>(pos, std::make_unique<ExtraBallEffect>());
        }
        return nullptr;
    }
}