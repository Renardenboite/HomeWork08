#pragma once
#include "GameObject.h"
#include "BonusEffect.h"
#include <memory>

namespace ArkanoidGame
{
    class Platform;

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

    private:
        std::unique_ptr<BonusEffect> effect;
        sf::Vector2f velocity = { 0.f, 150.f };
        bool destroyed = false;
    };
}