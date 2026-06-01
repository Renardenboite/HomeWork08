#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <algorithm>
#include <assert.h>

namespace
{
	const std::string TEXTURE_ID = "platform";
}

namespace ArkanoidGame
{
	void Platform::Init()
	{
		assert(texture.loadFromFile(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png"));

		InitSprite(sprite, SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT, texture);
		sprite.setPosition({ SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT / 2.f });
	}

	void Platform::Update(float timeDelta)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Move(-timeDelta * SETTINGS.PLATFORM_SPEED);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Move(timeDelta * SETTINGS.PLATFORM_SPEED);
		}
	}

	Platform::Platform(const sf::Vector2f& position)
	{
		assert(texture.loadFromFile(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png"));
		InitSprite(sprite, SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT, texture);
		sprite.setPosition(position);
	}

	bool Platform::CheckCollision(std::shared_ptr<Collidable> collidable)
	{
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball) return false;

		if (GetCollision(ball)) {
			auto rect = GetRect();
			float ballPosOnPlatform = (ball->GetPosition().x - (rect.left + rect.width / 2.f)) / (rect.width / 2.f);
			ball->ChangeAngle(90.f - 20.f * ballPosOnPlatform);
			return true;
		}
		return false;
	}

	bool Platform::GetCollision(std::shared_ptr<Collidable> collidable) const
	{
		auto ball = std::static_pointer_cast<Ball>(collidable);
		if (!ball) return false;
		return CheckCollisionWithBall(*ball);
	}

	void Platform::Draw(sf::RenderWindow& window)
	{
		DrawSprite(sprite, window);
	}
	void Platform::Move(float speed)
	{
		sf::Vector2f position = sprite.getPosition();
		float halfWidth = sprite.getGlobalBounds().width / 2.f;
		position.x = std::clamp(position.x + speed, halfWidth, SETTINGS.SCREEN_WIDTH - halfWidth);
		sprite.setPosition(position);
	}

	void Platform::restart()
	{
		sprite.setPosition({ SETTINGS.SCREEN_WIDTH / 2.f,
							 SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT / 2.f });
	}

	bool Platform::CheckCollisionWithBall(const Ball& ball) const
	{
		auto sqr = [](float x) { return x * x;  };

		const auto rect = sprite.getGlobalBounds();
		const sf::Vector2f ballPos = ball.GetPosition();

		if (ballPos.x < rect.left)
		{
			return sqr(ballPos.x - rect.left) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.f);
		}

		if (ballPos.x > rect.left + rect.width)
		{
			return sqr(ballPos.x - rect.left - rect.width) + sqr(ballPos.y - rect.top) < sqr(SETTINGS.BALL_SIZE / 2.f);
		}

		return std::fabs(ballPos.y - rect.top) <= SETTINGS.BALL_SIZE / 2.0;
	}
}