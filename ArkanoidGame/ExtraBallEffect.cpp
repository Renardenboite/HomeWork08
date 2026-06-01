#include "ExtraBallEffect.h"
#include "GameStatePlaying.h"
#include "Ball.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
    void ExtraBallEffect::Apply(GameStatePlayingData& game)
    {
        auto& balls = game.GetBalls();
        if (balls.empty()) return;

        auto& mainBall = balls.front();
        sf::Vector2f pos = mainBall->GetPosition();

        auto newBall = std::make_shared<Ball>(pos);
        newBall->ChangeAngle(135.f);
        newBall->AddObserver(game.weak_from_this());
        game.AddBall(newBall);
        extraBall = newBall;        
    }

    void ExtraBallEffect::Revert(GameStatePlayingData& game)
    {
        if (extraBall)
        {
            extraBall.reset();
        }
    }

    std::string ExtraBallEffect::GetDescription() const { return "Extra Ball!"; }
}