#pragma once
#include "Bonus.h"
#include <memory>

namespace ArkanoidGame 
{
    class BonusFactory
    {
    public:
        static std::unique_ptr<Bonus> CreateRandom(const sf::Vector2f& pos);
    };
}