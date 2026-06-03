#pragma once
#include "BonusEffect.h"
#include <memory>

namespace ArkanoidGame
{
    class Ball;

    class ExtraBallEffect : public BonusEffect
    {
    public:
        void Apply(GameStatePlayingData& game) override;
        void Revert(GameStatePlayingData& game) override;
        std::string GetDescription() const override;

        EffectType GetType() const override { return EffectType::ExtraBallEffect; }

    private:
        std::shared_ptr<Ball> extraBall;
    };
}