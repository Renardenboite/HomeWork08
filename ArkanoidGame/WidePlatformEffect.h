#pragma once
#include "BonusEffect.h"

namespace ArkanoidGame
{
    class WidePlatformEffect : public BonusEffect
    {
    public:
        void Apply(GameStatePlayingData& game) override;
        void Revert(GameStatePlayingData& game) override;
        std::string GetDescription() const override;        
    };
}