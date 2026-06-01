#include "ThreeHitBlock.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
    ThreeHitBlock::ThreeHitBlock(const sf::Vector2f& position, float width, float height, const sf::Color& color)
        : Block(position, width, height, color)
    {
        originalColor = color;
        hitCount = maxHits;
        LoadTextures();
        sprite.setTexture(hitTextures[0]);
    }

    void ThreeHitBlock::LoadTextures()
    {
        sf::Texture tex0, tex1, tex2, tex3;
        tex0.loadFromFile(SETTINGS.TEXTURES_PATH + "block_multi_0.png");
        tex1.loadFromFile(SETTINGS.TEXTURES_PATH + "block_multi_1.png");
        tex2.loadFromFile(SETTINGS.TEXTURES_PATH + "block_multi_2.png");
        tex3.loadFromFile(SETTINGS.TEXTURES_PATH + "block_multi_3.png");

        hitTextures.push_back(tex0);
        hitTextures.push_back(tex1);
        hitTextures.push_back(tex2);
        hitTextures.push_back(tex3);
    }

    void ThreeHitBlock::Update(float timeDelta)
    {
        if (isBreaking)
            UpdateTimer(timeDelta);
    }

    bool ThreeHitBlock::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        if (IsDestroyed() || isTimerStarted_)
            return false;

        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        return GetRect().intersects(gameObject->GetRect());
    }

    void ThreeHitBlock::OnHit()
    {
        --hitCount;
        ++currentHits;
        StageChange();

        if (hitCount <= 0)
        {
            isBreaking = true;
            StartTimer(SETTINGS.BREAK_DELAY);
        }
    }

    void ThreeHitBlock::Draw(sf::RenderWindow& window)
    {
        if (!IsDestroyed() || isTimerStarted_)
            DrawSprite(sprite, window);
    }

    void ThreeHitBlock::StageChange()
    {
        int hitsReceived = currentHits;
        if (hitsReceived >= 1 && hitsReceived <= static_cast<int>(hitTextures.size()))
            sprite.setTexture(hitTextures[hitsReceived]);

        if (hitsReceived == 1)
            sprite.setColor(sf::Color::Red);
        else if (hitsReceived == 2)
            sprite.setColor(sf::Color::Green);
    }

    void ThreeHitBlock::FinalAction()
    {
        hitCount = 0;
        Emit();
    }

    void ThreeHitBlock::EachTickAction(float deltaTime)
    {
        float alpha = originalColor.a * (currentTime_ / destroyTime_);
        sprite.setColor(sf::Color(originalColor.r, originalColor.g, originalColor.b,
            static_cast<sf::Uint8>(alpha)));
    }
}