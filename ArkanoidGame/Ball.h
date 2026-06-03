#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"
#include "IObserver.h"

namespace ArkanoidGame
{
    class Ball final : public GameObject, public Collidable, public IObservable
    {
    public:
        Ball(const sf::Vector2f& position);
        ~Ball() = default;
        void Update(float timeDelta) override;

        void InvertDirectionX();
        void InvertDirectionY();

        bool GetCollision(std::shared_ptr<Collidable> collidable) const override;
        void ChangeAngle(float x);
        void restart() override;
        void ChangeSpeed(float multipleSpeed);
        void SetDirection(const sf::Vector2f& newDir);

        float GetLastAngle() const { return lastAngle; }
        void AddAngleDeviation(float delta);
        void UpdateAngleFromDirection();
        float GetMultiplySpeed() const { return multiplySpeed; }

        const sf::Vector2f& GetDirection() const { return direction; }
        void SetAngle(float angle) { lastAngle = angle; }

    private:
        void OnHit() override {}
        sf::Vector2f direction;
        float lastAngle = 90;
        float multiplySpeed = 1.f;
    };
}