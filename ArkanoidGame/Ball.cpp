#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>
#include <algorithm>
#include "Randomizer.h"

namespace
{
	const std::string TEXTURE_ID = "ball";
}

namespace ArkanoidGame
{
	Ball::Ball(const sf::Vector2f& position)
		: GameObject(SETTINGS.TEXTURES_PATH + TEXTURE_ID + ".png", position, SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE)
	{
		const float angle = 45.f;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = -std::sin(pi / 180.f * angle);
		sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
		sprite.setPosition(position);
	}

	void Ball::ChangeSpeed(float multipleSpeed)
	{
		multiplySpeed = multipleSpeed;
	}
	
	void Ball::Update(float timeDelta)
	{
		timeDelta = multiplySpeed * timeDelta;
		const auto pos = sprite.getPosition() + SETTINGS.BALL_SPEED * timeDelta * direction;
		sprite.setPosition(pos);

		if (pos.x - SETTINGS.BALL_SIZE / 2.f <= 0 || pos.x + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_WIDTH) {
			direction.x *= -1;
		}

		if (pos.y - SETTINGS.BALL_SIZE / 2.f <= 0 /*|| pos.y + SETTINGS.BALL_SIZE / 2.f >= SETTINGS.SCREEN_HEIGHT*/) {
			direction.y *= -1;
		}

		Emit();
	}

	void Ball::InvertDirectionX()
	{
		direction.x *= -1; 

		float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= len;
		direction.y /= len;
	}

	void Ball::InvertDirectionY()
	{
		direction.y = -std::abs(direction.y);

		float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= len;
		direction.y /= len;
	}

	bool Ball::GetCollision(std::shared_ptr<Collidable> collidable) const
	{
		auto gameObject = std::dynamic_pointer_cast<GameObject>(collidable);
		assert(gameObject);
		return GetRect().intersects(gameObject->GetRect());
	}

	void Ball::ChangeAngle(float angle)
	{
		lastAngle = angle;
		const auto pi = std::acos(-1.f);
		direction.x = (angle / abs(angle)) * std::cos(pi / 180.f * angle);
		direction.y = -1 * abs(std::sin(pi / 180.f * angle));
	}

	void Ball::restart()
	{
		sprite.setPosition({ SETTINGS.SCREEN_WIDTH / 2.f,
							 SETTINGS.SCREEN_HEIGHT - SETTINGS.PLATFORM_HEIGHT - SETTINGS.BALL_SIZE / 2.f });
		const float angle = 45.f;
		const auto pi = std::acos(-1.f);
		direction.x = std::cos(pi / 180.f * angle);
		direction.y = -std::sin(pi / 180.f * angle);
		lastAngle = 45.f;
	}

	void Ball::SetDirection(const sf::Vector2f& newDir) 
	{
		direction = newDir;
		float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (len > 0.001f) { direction.x /= len; direction.y /= len; }
	}

	void Ball::UpdateAngleFromDirection()
	{
		const auto pi = std::acos(-1.f);
		float angle = std::atan2(direction.y, direction.x) * 180.f / pi;
		if (direction.x < 0)
			lastAngle = -std::abs(angle);
		else
			lastAngle = std::abs(angle);
	}

	void Ball::AddAngleDeviation(float delta)
	{
		float newAngle = lastAngle + delta;
		if (newAngle > 150.f) newAngle = 150.f;
		if (newAngle < 30.f) newAngle = 30.f;
		if (lastAngle < 0) newAngle = -newAngle;
		ChangeAngle(newAngle);
	}
}