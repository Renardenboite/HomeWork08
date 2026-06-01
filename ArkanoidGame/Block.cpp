#include "Block.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>

namespace
{
    const std::string TEXTURE_ID = "block_multi_0";
}

namespace ArkanoidGame
{
    Block::Block(const sf::Vector2f& position, float width, float height, const sf::Color& color)
        : GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, width, height)
    {
        sprite.setColor(color);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
        sprite.setPosition(position);
    }

    bool Block::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        sf::Rect rect = gameObject->GetRect();
        rect.width *= 1.1f;
        return GetRect().intersects(gameObject->GetRect());
    }

    void Block::Draw(sf::RenderWindow& window)
    {
        if (!IsDestroyed())
            DrawSprite(sprite, window);
    }

    void Block::OnHit()
    {
        hitCount = 0;
        Emit();
    }

    void Block::Update(float timeDelta) {}

    Block::~Block() {}

    SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position, float width, float height,
        const sf::Color& color)
        : Block(position, width, height, color), color(color)
    {
    }

    void SmoothDestroyableBlock::Update(float timeDelta)
    {
        UpdateTimer(timeDelta);
    }

    bool SmoothDestroyableBlock::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        if (IsDestroyed() || isTimerStarted_)
            return false;

        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        sf::Rect rect = gameObject->GetRect();
        rect.width *= 1.1f;
        return GetRect().intersects(gameObject->GetRect());
    }

    void SmoothDestroyableBlock::OnHit()
    {
        /*if (isTimerStarted_)
            return;*/
        hitCount = 0;
        StartTimer(SETTINGS.BREAK_DELAY);
        //Emit();
    }

    void SmoothDestroyableBlock::FinalAction()
    {
        hitCount = 0;
        Emit();
    }

    void SmoothDestroyableBlock::EachTickAction(float timeDelta)
    {
        color.a = static_cast<sf::Uint8>(255 * currentTime_ / destroyTime_);
        sprite.setColor(color);
    }

    void SmoothDestroyableBlock::Draw(sf::RenderWindow& window)
    {
        if (!IsDestroyed() || isTimerStarted_)
            DrawSprite(sprite, window);
    }

    UnbreackableBlock::UnbreackableBlock(const sf::Vector2f& position, float width, float height, const sf::Color&)
        : Block(position, width, height, sf::Color(105, 105, 105))
    {
    }

    void UnbreackableBlock::OnHit() {}

    GlassBlock::GlassBlock(const sf::Vector2f& position, float width, float height, const sf::Color& color)
        : Block(position, width, height, sf::Color(200, 220, 255, 180))
    {
        hitCount = 1;
        originalColor = sf::Color(200, 220, 255, 180);
    }

    void GlassBlock::Update(float timeDelta) {}

    bool GlassBlock::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        return GetRect().intersects(gameObject->GetRect());
    }

    void GlassBlock::OnHit()
    {
        hitCount = 0;
        //Emit();
    }

    void GlassBlock::EachTickAction(float timeDelta)
    {
        float alpha = originalColor.a * (currentTime_ / destroyTime_);
        sprite.setColor(sf::Color(
            originalColor.r,
            originalColor.g,
            originalColor.b,
            static_cast<sf::Uint8>(alpha)));
    }

    void GlassBlock::FinalAction()
    {
        hitCount = 0;
        Emit();
    }

    bool GlassBlock::IsDestroyed() const
    {
        return hitCount <= 0;
    }
}