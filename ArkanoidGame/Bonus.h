#pragma once
#include "GameObject.h"
#include "BonusEffect.h"
#include <memory>

namespace ArkanoidGame
{
    class Platform;

    enum class BonusType
    {
        WidePlatform,
        SlowBall,
        ExtraBall
    };

    class Bonus : public GameObject
    {
    public:
        Bonus(const sf::Vector2f& position, std::unique_ptr<BonusEffect> effect);
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool IsCaught(const Platform& platform) const;
        bool IsDestroyed() const { return destroyed; }
        void Destroy() { destroyed = true; }

        std::unique_ptr<BonusEffect> TakeEffect() { return std::move(effect); }

        BonusType GetType() const
        {
            if (effect)
            {
                switch (effect->GetType())
                {
                case EffectType::WidePlatform: return BonusType::WidePlatform;
                case EffectType::SlowBallEffect:    return BonusType::SlowBall;
                case EffectType::ExtraBallEffect:   return BonusType::ExtraBall;
                }
            }
            return BonusType::WidePlatform;
        }

        float GetRemainingTime() const { return lifetime; }
        void SetRemainingTime(float t) { lifetime = t; }

    private:
        std::unique_ptr<BonusEffect> effect;
        sf::Vector2f velocity = { 0.f, 150.f };
        bool destroyed = false;
        float lifetime = 0.f;
    };
}