#pragma once
#include <string>

namespace ArkanoidGame
{
    class GameStatePlayingData;

    enum class EffectType
    {
        WidePlatform,
        SlowBallEffect,
        ExtraBallEffect
    };

    class BonusEffect 
    {
    public:
        virtual ~BonusEffect() = default;
        virtual void Apply(GameStatePlayingData& game) = 0;
        virtual void Revert(GameStatePlayingData& game) = 0;
        virtual std::string GetDescription() const { return "Unknown Bonus"; }

        virtual EffectType GetType() const = 0;
    };
}