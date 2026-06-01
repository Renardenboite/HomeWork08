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
}