#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"
#include <algorithm>

namespace ArkanoidGame
{
	class Ball;

	class Platform : public GameObject, public Collidable
	{
	public:
		Platform() = default;
		Platform(const sf::Vector2f& position);

		void Init();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window) override;
		void OnHit() override {}

		void restart() override;
		bool GetCollision(std::shared_ptr<Collidable> collidable) const override;
		bool CheckCollision(std::shared_ptr<Collidable> collidable) override;
		bool CheckCollisionWithBall(const Ball& ball) const;
		float GetWidth() const { return sprite.getGlobalBounds().width; }
		void SetWidth(float newWidth) {
			sprite.setScale(newWidth / originalWidth, 1.f);
		}
		sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }

	private:
		void Move(float speed);
		float originalWidth = SETTINGS.PLATFORM_WIDTH;
	};
}