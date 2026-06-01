#include "Bonus.h"
#include "Platform.h"
#include "Sprite.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
    Bonus::Bonus(const sf::Vector2f& position, std::unique_ptr<BonusEffect> eff)
        : GameObject(SETTINGS.TEXTURES_PATH + "bonus.png", position, SETTINGS.BONUS_SIZE, SETTINGS.BONUS_SIZE),
        effect(std::move(eff))
    {
    }

    void Bonus::Update(float timeDelta)
    {
        sf::Vector2f pos = sprite.getPosition();
        pos += velocity * timeDelta;
        sprite.setPosition(pos);

        if (pos.y > SETTINGS.SCREEN_HEIGHT + 50.f)
        {
            destroyed = true;
        }
    }

    void Bonus::Draw(sf::RenderWindow& window)
    {
        if (!destroyed)
            DrawSprite(sprite, window);
    }

    bool Bonus::IsCaught(const Platform& platform) const
    {
        return sprite.getGlobalBounds().intersects(platform.GetRect());
    }
}