#include "SlowBallEffect.h"
#include "GameStatePlaying.h"
#include "Ball.h"

namespace ArkanoidGame
{
    void SlowBallEffect::Apply(GameStatePlayingData& game)
    {
        for (auto& ball : game.GetBalls())
        {
            if (originalMultipliers.find(ball.get()) == originalMultipliers.end())
            {
                originalMultipliers[ball.get()] = ball->GetMultiplySpeed();
            }
            float newMultiplier = std::max(0.3f, originalMultipliers[ball.get()] * 0.7f);
            ball->ChangeSpeed(newMultiplier);
        }
    }

    void SlowBallEffect::Revert(GameStatePlayingData& game)
    {
        for (auto& ball : game.GetBalls())
        {
            auto it = originalMultipliers.find(ball.get());
            if (it != originalMultipliers.end())
            {
                ball->ChangeSpeed(it->second);
            }
            else
            {
                ball->ChangeSpeed(1.0f);
            }
        }
        originalMultipliers.clear();
    }

    std::string SlowBallEffect::GetDescription() const { return "Slow Ball!"; }
}