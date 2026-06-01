#pragma once
#include "BonusEffect.h"
#include <unordered_map>
#include "Ball.h"

namespace ArkanoidGame
{
    class SlowBallEffect : public BonusEffect
    {
    public:
        void Apply(GameStatePlayingData& game) override;
        void Revert(GameStatePlayingData& game) override;
        std::string GetDescription() const override;

    private:
        std::unordered_map<Ball*, float> originalMultipliers;
    };
}