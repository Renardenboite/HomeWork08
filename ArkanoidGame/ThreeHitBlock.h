#pragma once
#include "Block.h"
#include "IDelayedAction.h"

namespace ArkanoidGame {
    class ThreeHitBlock : public Block, public IDelayedAction
    {
    public:
        ThreeHitBlock(const sf::Vector2f& position, float width, float height,
            const sf::Color& color = sf::Color::Magenta);
        void Update(float timeDelta) override;
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        void Draw(sf::RenderWindow& window) override;

    protected:
        void OnHit() override;
        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

    private:
        void StageChange();
        void LoadTextures();

        int maxHits = 3;
        int currentHits = 0;
        std::vector<sf::Texture> hitTextures;
        sf::Color originalColor;
        bool isBreaking = false;
    };
}