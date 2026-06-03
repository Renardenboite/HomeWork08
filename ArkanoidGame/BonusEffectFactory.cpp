#include "BonusEffectFactory.h"
#include "WidePlatformEffect.h"
#include "SlowBallEffect.h"
#include "ExtraBallEffect.h"

namespace ArkanoidGame
{
    std::unique_ptr<BonusEffect> BonusEffectFactory::Create(EffectType type)
    {
        switch (type)
        {
        case EffectType::WidePlatform:
            return std::make_unique<WidePlatformEffect>();
        case EffectType::SlowBallEffect:
            return std::make_unique<SlowBallEffect>();
        case EffectType::ExtraBallEffect:
            return std::make_unique<ExtraBallEffect>();
        default:
            return nullptr;
        }
    }
}