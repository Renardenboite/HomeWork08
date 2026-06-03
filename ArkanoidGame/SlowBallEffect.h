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

        EffectType GetType() const override { return EffectType::SlowBallEffect; }

    private:
        std::unordered_map<Ball*, float> originalMultipliers;
    };
}