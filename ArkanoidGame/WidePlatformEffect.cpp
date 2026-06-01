#include "WidePlatformEffect.h"
#include "GameStatePlaying.h"
#include "Platform.h"

namespace ArkanoidGame
{
    void WidePlatformEffect::Apply(GameStatePlayingData& game)
    {
        Platform* platform = game.GetPlatform();
        if (!platform) return;

        const float originalWidth = SETTINGS.PLATFORM_WIDTH;
        platform->SetWidth(originalWidth * 1.5f);
    }

    void WidePlatformEffect::Revert(GameStatePlayingData& game)
    {
        Platform* platform = game.GetPlatform();
        if (!platform) return;

        platform->SetWidth(SETTINGS.PLATFORM_WIDTH);
    }

    std::string WidePlatformEffect::GetDescription() const { return "Wide Platform!"; }
}