#pragma once
#include "BonusEffect.h"
#include <memory>

namespace ArkanoidGame
{
    class BonusEffectFactory
    {
    public:
        static std::unique_ptr<BonusEffect> Create(EffectType type);
    };
}