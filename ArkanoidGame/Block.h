#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include "IDelayedAction.h"
#include "IObserver.h"
#include <vector>

namespace ArkanoidGame
{
    class Block : public GameObject, public Collidable, public IObservable
    {
    public:
        Block() = default;
        Block(const sf::Vector2f& position, float width, float height,
            const sf::Color& color = sf::Color::Red);
        virtual ~Block();

        void Draw(sf::RenderWindow& window) override;
        void Update(float timeDelta) override;
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;

        virtual bool IsDestroyed() const { return hitCount <= 0; }
        virtual void OnHit();
        virtual int GetScoreValue() const { return 10; }
        virtual bool IsTimerStarted() const { return false; }

        virtual void Destroy() { hitCount = 0; }

        int hitCount = 1;

    protected:
        bool destroyed = false;
    };

    class SmoothDestroyableBlock : public Block, public IDelayedAction
    {
    protected:
        void OnHit() override;
        sf::Color color;

    public:
        SmoothDestroyableBlock(const sf::Vector2f& position, float width, float height,
            const sf::Color& color = sf::Color::Blue);
        ~SmoothDestroyableBlock() = default;

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        bool IsDestroyed() const override { return hitCount <= 0 && !isTimerStarted_; }
        void FinalAction() override;
        void EachTickAction(float deltaTime) override;
        bool IsTimerStarted() const override { return isTimerStarted_; }

        int GetScoreValue() const override { return 20; }
    };

    class UnbreackableBlock : public Block
    {
    public:
        UnbreackableBlock(const sf::Vector2f& position, float width, float height,
            const sf::Color& color = sf::Color(105, 105, 105));
        void OnHit() override;
        void Update(float timeDelta) override {}

        int GetScoreValue() const override { return 0; }
    };

    class GlassBlock : public Block, public IDelayedAction
    {
    public:
        GlassBlock(const sf::Vector2f& position, float width, float height,
            const sf::Color& color = sf::Color(200, 220, 255, 180));
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        void OnHit() override;
        bool IsDestroyed() const override;
        void Update(float timeDelta) override;

        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

        int GetScoreValue() const override { return 30; }

    private:
        mutable bool collisionProcessed = false;
        sf::Color originalColor;
    };
}